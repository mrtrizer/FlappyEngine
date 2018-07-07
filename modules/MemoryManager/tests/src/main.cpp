#include <iostream>
#include <vector>
#include <unordered_set>
#include <cassert>
#include <sstream>
#include <memory>
#include <string>

class FlappyException : public std::exception {
public:
    FlappyException(std::string message) noexcept : m_message(std::move(message))  {
        // TODO: Stack trace
    }

    const char* what() const noexcept override {
        return m_message.c_str();
    }

private:
    std::string m_message;
};

// Use USER_ASSERT assert in public methods or methods, called from public methods.
// It throws an exception for wrong statement. Use when case is exceptional but execution can be continued.
#define USER_ASSERT(statement) { if (!(statement)) { throw FlappyException(std::string(__FILE__) + std::to_string(__LINE__)); } }

// Use DEBUG_ASSERT only for next applications:
// 1. Check class has valid state.
// 2. Error can't be handled. Like memory is broken.
// Notice that DEBUG_ASSERT is turned off in production.
#ifdef NDEBUG
    #define DEBUG_ASSERT (void)
#else
// FIXME: Use std::terminate and custom output
    #define DEBUG_ASSERT(statement) assert(statement && __FILE__ && __LINE__)
#endif

template <typename... Args>
std::string sstr(Args &&... args)
{
    std::ostringstream sstr;
    (sstr << ... << args);
    return sstr.str();
}

template <typename DataT>
class StrongHandle;

template <typename DataT>
class IHandle {
    friend class StrongHandle<DataT>; // to invalidate and update handle
public:
    virtual ~IHandle() = default;
private:
    virtual void invalidate() noexcept = 0;
    virtual void updateStrongHandle(StrongHandle<DataT>* strongHandle) noexcept = 0;
};

template <typename DataT>
class ChankArray;

template <typename DataT>
class Handle;

template <typename DataT>
class StrongHandle {
    friend class ChankArray<DataT>; // to update pointer
    friend class Handle<DataT>; // to register/unregister of handles
public:
    StrongHandle(StrongHandle&& strongHandle) noexcept {
        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        m_removeCallback = strongHandle.m_removeCallback;
        m_updateCallback = strongHandle.m_updateCallback;
        auto& handles = m_handles = std::move(strongHandle.m_handles);
        for (auto handle : handles)
            handle->updateStrongHandle(this);
        m_updateCallback(this);
    }
    StrongHandle& operator=(StrongHandle&&) = delete;
    StrongHandle(const StrongHandle&) = delete;
    StrongHandle& operator=(const StrongHandle&) = delete;
    ~StrongHandle() {
        if (m_dataPointer == nullptr)
            return;
        for (auto handle : m_handles)
            handle->invalidate();
        try {
            m_removeCallback();
        } catch (...) {
            DEBUG_ASSERT(false);
        }
    }

    DataT* operator->() const noexcept {
        DEBUG_ASSERT(m_dataPointer != nullptr);

        return m_dataPointer;
    }

private:
    DataT* m_dataPointer = nullptr;
    std::function<void(void)> m_removeCallback;
    std::function<void(StrongHandle*)> m_updateCallback;
    std::vector<IHandle<DataT>*> m_handles;

    StrongHandle(DataT* dataPointer,
                 std::function<void(void)>&& removeCallback,
                 std::function<void(StrongHandle*)>&& updateCallback) noexcept
        : m_dataPointer(dataPointer)
        , m_removeCallback(std::move(removeCallback))
        , m_updateCallback(std::move(updateCallback))
    {}

    void updatePointer(DataT* dataPointer,
                       const std::function<void(void)>& removeCallback,
                       const std::function<void(StrongHandle*)>& updateCallback) noexcept {
        DEBUG_ASSERT(dataPointer != nullptr);
        DEBUG_ASSERT(removeCallback != nullptr);
        DEBUG_ASSERT(updateCallback != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(m_removeCallback != nullptr);
        DEBUG_ASSERT(m_updateCallback != nullptr);

        m_dataPointer = dataPointer;
        m_removeCallback = removeCallback;
        m_updateCallback = updateCallback;
    }

    void registerHandle(IHandle<DataT>* handle) noexcept {
        DEBUG_ASSERT(handle != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);

        m_handles.emplace_back(handle);
    }

    void unregisterHandle(IHandle<DataT>* handle) noexcept {
        // TODO: A room for optimization
        DEBUG_ASSERT(handle != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(!m_handles.empty());

        if (m_handles.back() != handle) {
            auto iter = std::find(m_handles.begin(), m_handles.end(), handle);
            DEBUG_ASSERT(iter != m_handles.end());
            *iter = m_handles.back();
        }

        m_handles.erase(std::prev(m_handles.end()), m_handles.end());
    }
};

template <typename DataT>
class Handle : public IHandle<DataT> {
public:
    Handle() noexcept {}

    explicit Handle(StrongHandle<DataT>& strongHandle) noexcept
        : m_strongHandle(const_cast<StrongHandle<DataT>*>(&strongHandle))
    {
        m_strongHandle->registerHandle(this);
    }

    Handle(std::nullptr_t)
    {}

    // Copy constructor/assignment
    Handle(const Handle& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    Handle& operator=(const Handle& handle) {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    Handle& operator=(StrongHandle<DataT>& handle) {
        setNewHandle(&handle);
        return *this;
    }

    // Move constructor/assignment
    Handle(Handle&& handle) {
        setNewHandle(handle.m_strongHandle);
    }

    Handle& operator=(Handle&& handle) {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    ~Handle() {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
    }

    DataT* operator->() const {
        if (m_strongHandle == nullptr)
            throw FlappyException("Invalid handle");
        return m_strongHandle->operator->();
    }

private:
    StrongHandle<DataT>* m_strongHandle = nullptr;

    void setNewHandle(StrongHandle<DataT>* strongHandle) {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
        m_strongHandle = strongHandle;
        if (strongHandle != nullptr)
            strongHandle->registerHandle(this);
    }

    void invalidate() noexcept override {
        DEBUG_ASSERT(m_strongHandle != nullptr);

        m_strongHandle = nullptr;
    }

    void updateStrongHandle(StrongHandle<DataT>* strongHandle) noexcept override {
        DEBUG_ASSERT(m_strongHandle != nullptr);
        DEBUG_ASSERT(strongHandle != nullptr);

        m_strongHandle = strongHandle;
    }
};

template <typename DataT>
class Chank {
public:
    Chank(DataT&& data)
        : m_data(std::move(data))
    {}

    void setStrongHandle(StrongHandle<DataT>* handle) noexcept { m_strongHandle = handle; }
    StrongHandle<DataT>* strongHandle() noexcept { return m_strongHandle; }

    DataT& data() noexcept { return m_data; }

private:
    StrongHandle<DataT>* m_strongHandle = nullptr;
    DataT m_data;
};

template <typename DataT>
class ChankArray {
    using Allocator = std::allocator<Chank<DataT>>;
    using AllocatorTraits = std::allocator_traits<Allocator>;
public:
    ChankArray(size_t capacity)
        : m_capacity(capacity)
    {
        USER_ASSERT(capacity > 0);

        m_end = m_first = AllocatorTraits::allocate(m_allocator, capacity);
    }
    ChankArray(const ChankArray&) = delete;
    ChankArray& operator= (const ChankArray&) = delete;
    ChankArray(ChankArray&& chankArray) = delete;
    ChankArray& operator= (ChankArray&&) = default;
    ~ChankArray() {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        for (auto pointer = m_first; pointer != m_end; ++pointer)
            AllocatorTraits::destroy(m_allocator, pointer);
        AllocatorTraits::deallocate(m_allocator, m_first, m_capacity);
    }

    template <typename...Args>
    StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ChankArray doesn't support basic types.");

        if (m_length >= m_capacity)
            throw FlappyException(sstr("You have reached limit of chanks. Max: ", m_capacity));

        auto chank = m_end;
        AllocatorTraits::construct(m_allocator, chank, DataT(std::forward<Args>(args)...));
        StrongHandle<DataT> strongHandle(&chank->data(),
                                         [this, chank] { remove(chank); },
                                         [this, chank] (StrongHandle<DataT>* newStrongHandle) { chank->setStrongHandle(newStrongHandle); });
        chank->setStrongHandle(&strongHandle);
        ++m_end;
        ++m_length;
        return strongHandle;
    }

private:
    size_t m_capacity = 0;
    size_t m_length = 0;
    Allocator m_allocator;
    Chank<DataT>* m_first = nullptr;
    Chank<DataT>* m_end = nullptr;

    void updatePointer(StrongHandle<DataT>* strongHandle, const Chank<DataT>* chank) {
        DEBUG_ASSERT(strongHandle != nullptr);
        DEBUG_ASSERT(chank != nullptr);

        strongHandle->updatePointer(
                    chank->data(),
                    [this, chank] { remove(chank); },
                    [this, chank] (StrongHandle<DataT>* newStrongHandle) { chank->setStrongHandle(newStrongHandle); }
        );
    }

    void remove(Chank<DataT>* chank) {
        DEBUG_ASSERT(m_length > 0);
        DEBUG_ASSERT(m_end != nullptr);

        AllocatorTraits::destroy(m_allocator, chank);
        auto last = m_end - 1;
        if (m_length > 1 && chank != std::prev(m_end))
            AllocatorTraits::construct(m_allocator, chank, std::move(*last));
        AllocatorTraits::destroy(m_allocator, last);
        --m_length;
        --m_end;
    }
};

class Test {
public:
    Test(int i) : m_i(i) {}
    int value() { return m_i; }
private:
    int m_i = 0;
};

int main()
{
    ChankArray<Test> chankArray(10);
    {
        //auto a = chankArray.create(10);
        //std::cout << a->value() << std::endl;
        Handle<Test> b = nullptr;
        {
            auto strongHandle = chankArray.create(20);
            b = strongHandle;
            auto otherStrongHandle = std::move(strongHandle);
            std::cout << b->value() << std::endl;
        }
        try {
            std::cout << b->value() << std::endl;
        } catch (std::exception& e) {

        }
        auto c = chankArray.create(30);
        std::cout << c->value() << std::endl;
    }

    return 0;
}
