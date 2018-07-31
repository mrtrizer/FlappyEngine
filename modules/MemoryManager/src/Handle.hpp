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

    template <typename DerivedT>
    Handle(StrongHandle<DerivedT>& strongHandle) noexcept
        : m_strongHandle(reinterpret_cast<StrongHandle<DataT>*>(&strongHandle))
    {
        static_assert(std::is_base_of<DataT, DerivedT>::value, "DerivedT should be derived from BaseT");
        m_strongHandle->registerHandle(this);
    }

    template <typename DerivedT>
    Handle& operator=(StrongHandle<DerivedT>& handle) noexcept {
        setNewHandle(&handle);
        return *this;
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
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    Handle& operator=(Handle&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
        return *this;
    }

    template <typename DerivedT>
    Handle(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
    }

    template <typename DerivedT>
    Handle& operator=(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename DerivedT>
    Handle(Handle<DerivedT>&& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    template <typename DerivedT>
    Handle& operator=(Handle<DerivedT>&& handle) noexcept {
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

    template <typename DerivedT>
    void setNewHandle(StrongHandle<DerivedT>* strongHandle) noexcept {
        static_assert(std::is_base_of<DataT, DerivedT>::value, "DerivedT should be derived from BaseT");
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
        m_strongHandle = reinterpret_cast<StrongHandle<DataT>*>(strongHandle);
        if (strongHandle != nullptr)
            strongHandle->registerHandle(reinterpret_cast<Handle<DerivedT>*>(this));
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
