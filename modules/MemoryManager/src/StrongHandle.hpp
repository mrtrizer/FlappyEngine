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

class Chank;

template <typename DataT>
class StrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class Chank; // to construct
    template<typename T>
    friend class ChankFunctions; // to update pointer
    friend class Handle<DataT>; // to register/unregister handles
public:
    StrongHandle(StrongHandle&& strongHandle) noexcept {
        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        auto& handles = m_handles = std::move(strongHandle.m_handles);
        for (auto handle : handles)
            handle->updateStrongHandle(this);
        [](auto chank, StrongHandle* strongHandle) { chank->m_strongHandle = strongHandle; } (m_chank, this);
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
            if (m_chank != nullptr)
                [](auto chank) { chank->clear(); } (m_chank);
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
    Chank* m_chank = nullptr;
    std::vector<IHandle<DataT>*> m_handles;

    StrongHandle(DataT* dataPointer,
                 Chank* chank) noexcept
        : m_dataPointer(dataPointer)
        , m_chank(chank)
    {}

    void updatePointer(DataT* dataPointer, Chank* chank) noexcept {
        DEBUG_ASSERT(dataPointer != nullptr);
        DEBUG_ASSERT(chank != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(m_chank != nullptr);

        m_dataPointer = dataPointer;
        m_chank = chank;
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
