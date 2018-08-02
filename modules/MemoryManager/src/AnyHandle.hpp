#pragma once

#include "AnyStrongHandle.hpp"

template <typename T>
class Handle;

class AnyHandle {
    friend class AnyStrongHandle; // to access invalidate() and setNewHandle()
public:
    AnyHandle(AnyStrongHandle& strongHandle) noexcept
        : m_strongHandle(&strongHandle)
    {
        m_strongHandle->registerHandle(this);
    }

    AnyHandle& operator=(AnyStrongHandle& strongHandle) noexcept {
        setNewHandle(&strongHandle);
        return *this;
    }

    AnyHandle(std::nullptr_t) noexcept
    {}

    template <typename DerivedT>
    AnyHandle(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    template <typename DerivedT>
    AnyHandle& operator=(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename DerivedT>
    AnyHandle(Handle<DerivedT>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    template <typename DerivedT>
    AnyHandle& operator=(Handle<DerivedT>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
        return *this;
    }

    ~AnyHandle() {
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
    AnyStrongHandle* m_strongHandle = nullptr;

    void setNewHandle(AnyStrongHandle* strongHandle) noexcept {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
        m_strongHandle = reinterpret_cast<AnyStrongHandle*>(strongHandle);
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

        m_strongHandle = static_cast<AnyStrongHandle*>(strongHandlePtr);
    }
};
