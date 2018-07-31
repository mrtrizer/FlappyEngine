#pragma once

#include <iostream>
#include <memory>

#include "StrongHandle.hpp"

// TODO: Anonimous handles

template <typename DataT>
class Handle : public IHandle<DataT> {
    template <typename T>
    friend class Handle;
public:
    Handle() noexcept {}

    template <typename T>
    explicit Handle(StrongHandle<T>& strongHandle) noexcept
        : m_strongHandle(reinterpret_cast<StrongHandle<DataT>*>(&strongHandle))
    {
        static_assert(std::is_base_of<DataT, T>::value, "DerivedT should be derived from BaseT");
        m_strongHandle->registerHandle(this);
    }

    Handle(std::nullptr_t) noexcept
    {}

    Handle(const Handle& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    Handle& operator=(const Handle& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    Handle(Handle&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    Handle& operator=(Handle&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename T>
    Handle(const Handle<T>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    template <typename T>
    Handle& operator=(const Handle<T>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename T>
    Handle& operator=(StrongHandle<T>& handle) noexcept {
        setNewHandle(&handle);
        return *this;
    }

    template <typename T>
    Handle(Handle<T>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    template <typename T>
    Handle& operator=(Handle<T>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
        return *this;
    }

    ~Handle() {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
    }

    bool isValid() noexcept {
        return m_strongHandle != nullptr && m_strongHandle->isValid();
    }

    DataT* operator->() {
        if (m_strongHandle == nullptr)
            throw FlappyException("Invalid handle");
        return m_strongHandle->operator->();
    }

private:
    StrongHandle<DataT>* m_strongHandle = nullptr;

    template <typename T>
    void setNewHandle(StrongHandle<T>* strongHandle) noexcept {
        static_assert(std::is_base_of<DataT, T>::value, "DerivedT should be derived from BaseT");
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
        m_strongHandle = reinterpret_cast<StrongHandle<DataT>*>(strongHandle);
        if (strongHandle != nullptr)
            strongHandle->registerHandle(reinterpret_cast<Handle<T>*>(this));
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
