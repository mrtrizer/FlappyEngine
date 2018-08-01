#pragma once

#include <vector>

#include "Utility.hpp"

template <typename DataT>
class StrongHandle;

template <typename DataT>
class Handle;

class Chank;

struct HandleCalls {
    void* rawPointer;
    std::function<void(void)> invalidate;
    std::function<void(void* strongHandle)> updateStrongHandle;
};

class StrongHandleBase {
    template <typename T>
    friend class Handle; // to register/unregister handles
    friend class UnknownHandle; // to register/unregister handles
public:
    ~StrongHandleBase() {
        reset();
    }

    bool isValid() const noexcept {
        return m_dataPointer != nullptr;
    }

    TypeId typeId() const noexcept{
        return m_typeId;
    }

protected:
    TypeId m_typeId;
    void* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;
    std::vector<HandleCalls> m_handles;

    StrongHandleBase(TypeId typeId = 0,
                     void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept
        : m_typeId(typeId)
        , m_dataPointer(dataPointer)
        , m_chank(chank)
    {}

    void reset() noexcept {
        for (auto handle : m_handles)
            handle.invalidate();
        m_handles.clear();
        if (m_chank != nullptr)
            clearChank (m_chank);
        m_chank = nullptr;
        m_dataPointer = nullptr;
    }

    template <typename T>
    void registerHandle(T* handle) noexcept {
        DEBUG_ASSERT(handle != nullptr);

       m_handles.emplace_back(HandleCalls{
           handle,
           std::bind(&T::invalidate, handle),
           std::bind(&T::updateStrongHandle, handle, std::placeholders::_1)
       });
    }

    void unregisterHandle(void* handle) noexcept {
        // TODO: A room for optimization
        DEBUG_ASSERT(handle != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(!m_handles.empty());

        if (m_handles.back().rawPointer != handle) {
            auto iter = std::find_if(m_handles.begin(), m_handles.end(), [handle](const auto& item) {
                return item.rawPointer == handle;
            });
            DEBUG_ASSERT(iter != m_handles.end());
            *iter = std::move(m_handles.back());
        }

        m_handles.erase(std::prev(m_handles.end()), m_handles.end());
    }

    template <typename DerivedT>
    void moveFromStrongHandle(StrongHandle<DerivedT>&& strongHandle) {
        m_typeId = strongHandle.m_typeId;
        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        auto chank = m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        std::move(strongHandle.m_handles.begin(), strongHandle.m_handles.end(), std::back_inserter(m_handles));
        strongHandle.m_handles.clear();
        for (auto handle : m_handles)
            handle.updateStrongHandle(this);
        if (chank != nullptr)
            updateChankStrongHandle (chank, this);
    }

    void updatePointer(void* dataPointer, Chank* chank) noexcept {
        DEBUG_ASSERT(dataPointer != nullptr);
        DEBUG_ASSERT(chank != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(m_chank != nullptr);

        m_dataPointer = dataPointer;
        m_chank = chank;
    }

private:
    template <typename ChankT>
    void clearChank(ChankT* chank) {
        chank->clear();
    }

    template <typename ChankT>
    void updateChankStrongHandle(ChankT* chank, StrongHandleBase* strongHandle) {
        chank->m_strongHandle = strongHandle;
    }
};

template <typename DataT>
class StrongHandle : public StrongHandleBase {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class Chank; // to construct
    template<typename T>
    friend class ChankFunctions; // to update pointer
    template <typename T>
    friend class StrongHandle;
public:
    StrongHandle& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    template <typename DerivedT>
    StrongHandle(StrongHandle<DerivedT>&& strongHandle) noexcept {
        checkType<DerivedT>();
        moveFromStrongHandle(std::move(strongHandle));
    }

    template <typename DerivedT>
    StrongHandle& operator=(StrongHandle<DerivedT>&& strongHandle) noexcept {
        checkType<DerivedT>();
        moveFromStrongHandle(std::move(strongHandle));
        return *this;
    }

    StrongHandle(StrongHandle&& strongHandle) noexcept {
        moveFromStrongHandle(std::move(strongHandle));
    }

    StrongHandle& operator=(StrongHandle&& strongHandle) noexcept {
        moveFromStrongHandle(std::move(strongHandle));
        return *this;
    }

    StrongHandle(const StrongHandle&) = delete;
    StrongHandle& operator=(const StrongHandle&) = delete;

    Handle<DataT> handle() noexcept {
        return Handle<DataT>(*this);
    }

    DataT* operator->() {
        if (!isValid())
            throw FlappyException("Invalid handle");
        return static_cast<DataT*>(m_dataPointer);
    }

private:
    StrongHandle(DataT* dataPointer,
                 Chank* chank) noexcept
        : StrongHandleBase(getTypeId<DataT>(), dataPointer, chank)
    {}

    template <typename DerivedT>
    constexpr void checkType() {
        static_assert(std::is_base_of<DataT, DerivedT>::value, "DerivedT should be derived from BaseT");
    }
};
