#pragma once

#include <vector>

#include "Utility.hpp"


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
class Handle;

template <typename DataT>
class StrongHandle {
    template<size_t ChankSize>
    friend class ChankArray; // to update pointer
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
