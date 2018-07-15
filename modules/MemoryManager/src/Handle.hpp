#pragma once

#include <iostream>
#include <memory>

#include "StrongHandle.hpp"

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

    bool isValid() {
        return m_strongHandle != nullptr && m_strongHandle->isValid();
    }

    DataT* operator->() {
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
