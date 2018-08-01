#pragma once

#include "StrongHandle.hpp"

template <typename T>
class Handle;

class UnknownHandle {
    friend class StrongHandleBase; // to access invalidate() and setNewHandle()
public:
    UnknownHandle(StrongHandleBase& strongHandle) noexcept
        : m_strongHandle(&strongHandle)
    {
        m_strongHandle->registerHandle(this);
    }

    UnknownHandle& operator=(StrongHandleBase& strongHandle) noexcept {
        setNewHandle(&strongHandle);
        return *this;
    }

    UnknownHandle(std::nullptr_t) noexcept
    {}

    template <typename DerivedT>
    UnknownHandle(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    template <typename DerivedT>
    UnknownHandle& operator=(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename DerivedT>
    UnknownHandle(Handle<DerivedT>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    template <typename DerivedT>
    UnknownHandle& operator=(Handle<DerivedT>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
        return *this;
    }

    ~UnknownHandle() {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
    }

    bool isValid() noexcept {
        return m_strongHandle != nullptr && m_strongHandle->isValid();
    }

    template <typename DataT>
    Handle<DataT> get() {
        // FIXME: If inherit Handle from UnknownHandle, it would be possible just to cast this* pointer
        if (m_strongHandle->typeId() == getTypeId<DataT>())
            return static_cast<StrongHandle<DataT>*>(m_strongHandle)->handle();
        else
            throw FlappyException("UnknownHandle points to another type.");
    }

private:
    StrongHandleBase* m_strongHandle = nullptr;

    void setNewHandle(StrongHandleBase* strongHandle) noexcept {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
        m_strongHandle = reinterpret_cast<StrongHandleBase*>(strongHandle);
        if (strongHandle != nullptr)
            strongHandle->registerHandle(this);
    }

    void invalidate() noexcept {
        DEBUG_ASSERT(m_strongHandle != nullptr);

        m_strongHandle = nullptr;
    }

    // strongHandlePtr is void* to support anonymous handles
    void updateStrongHandle(void* strongHandlePtr) noexcept {
        DEBUG_ASSERT(m_strongHandle != nullptr);
        DEBUG_ASSERT(strongHandlePtr != nullptr);

        m_strongHandle = static_cast<StrongHandleBase*>(strongHandlePtr);
    }
};
