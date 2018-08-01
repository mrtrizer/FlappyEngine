#pragma once

#include <vector>

#include "Utility.hpp"

// TODO: Anonimous strong handles

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

template <typename DataT>
class StrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class Chank; // to construct
    template<typename T>
    friend class ChankFunctions; // to update pointer
    template <typename T>
    friend class Handle; // to register/unregister handles
    template <typename T>
    friend class StrongHandle;
public:
    StrongHandle& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    template <typename DerivedT>
    StrongHandle(StrongHandle<DerivedT>&& strongHandle) noexcept {
        moveFromStrongHandle(std::move(strongHandle));
    }

    template <typename DerivedT>
    StrongHandle& operator=(StrongHandle<DerivedT>&& strongHandle) noexcept {
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
    ~StrongHandle() {
        reset();
    }

    Handle<DataT> handle() noexcept {
        return Handle<DataT>(*this);
    }

    bool isValid() const noexcept {
        return m_dataPointer != nullptr;
    }

    void reset() noexcept {
        for (auto handle : m_handles)
            handle.invalidate();
        m_handles.clear();
        if (m_chank != nullptr)
            [](auto chank) { chank->clear(); } (m_chank);
        m_chank = nullptr;
        m_dataPointer = nullptr;
    }

    DataT* operator->() {
        if (!isValid())
            throw FlappyException("Invalid handle");
        return m_dataPointer;
    }

private:
    DataT* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;
    std::vector<HandleCalls> m_handles;

    StrongHandle(DataT* dataPointer,
                 Chank* chank) noexcept
        : m_dataPointer(dataPointer)
        , m_chank(chank)
    {}

    template <typename DerivedT>
    void moveFromStrongHandle(StrongHandle<DerivedT>&& strongHandle) {
        static_assert(std::is_base_of<DataT, DerivedT>::value, "DerivedT should be derived from BaseT");

        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        auto chank = m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        for (auto handleCalls : strongHandle.m_handles)
            m_handles.emplace_back(handleCalls);
        strongHandle.m_handles.clear();
        for (auto handle : m_handles)
            handle.updateStrongHandle(this);
        if (chank != nullptr)
            [](auto chank, StrongHandle* strongHandle) { chank->m_strongHandle = strongHandle; } (chank, this);
    }

    void updatePointer(DataT* dataPointer, Chank* chank) noexcept {
        DEBUG_ASSERT(dataPointer != nullptr);
        DEBUG_ASSERT(chank != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(m_chank != nullptr);

        m_dataPointer = dataPointer;
        m_chank = chank;
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

    void unregisterHandle(Handle<DataT>* handle) noexcept {
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
};
