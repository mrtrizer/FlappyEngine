#pragma once

#include <vector>

#include "Utility.hpp"

// TODO: Anonimous strong handles

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
        auto chank = m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        auto& handles = m_handles = std::move(strongHandle.m_handles);
        for (auto handle : handles)
            handle->updateStrongHandle(this);
        if (chank != nullptr)
            [](auto chank, StrongHandle* strongHandle) { chank->m_strongHandle = strongHandle; } (chank, this);
    }
    StrongHandle& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }
    StrongHandle& operator=(StrongHandle&& strongHandle) {
        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        auto chank = m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        for (auto handle : strongHandle.m_handles)
            handle->updateStrongHandle(this);
        m_handles.insert(m_handles.end(),
                         std::make_move_iterator(strongHandle.m_handles.begin()),
                         std::make_move_iterator(strongHandle.m_handles.end()));
        if (chank != nullptr)
            [](auto chank, StrongHandle* strongHandle) { chank->m_strongHandle = strongHandle; } (chank, this);
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
            handle->invalidate();
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
