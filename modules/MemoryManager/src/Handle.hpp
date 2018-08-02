#pragma once

#include <iostream>
#include <memory>

#include "StrongHandle.hpp"
#include "AnyHandle.hpp"

template <typename DataT>
class Handle : public AnyHandle {
    template <typename T>
    friend class Handle; // for interided type convertion
    template <typename T>
    friend class StrongHandle; // for access to invalidate() and setNewHandle()
    friend class AnyStrongHandle; // for access to invalidate() and setNewHandle()
public:
    Handle() = default;

    template <typename DerivedT>
    Handle(StrongHandle<DerivedT>& strongHandle) noexcept
        : AnyHandle(strongHandle)
    {
        assertDerived<DataT, DerivedT>();
    }

    template <typename DerivedT>
    Handle& operator=(StrongHandle<DerivedT>& strongHandle) noexcept {
        assertDerived<DataT, DerivedT>();
        AnyHandle::operator=(strongHandle);
        return *this;
    }

    template <typename DerivedT>
    Handle(const Handle<DerivedT>& handle, int i = 0) noexcept
        : AnyHandle(handle)
    {
        assertDerived<DataT, DerivedT>();
    }

    template <typename DerivedT>
    Handle& operator=(const Handle<DerivedT>& handle) noexcept {
        assertDerived<DataT, DerivedT>();
        AnyHandle::operator=(handle);
        return *this;
    }

    template <typename DerivedT>
    Handle(Handle<DerivedT>&& handle, int i = 0) noexcept
        : AnyHandle(std::move(handle))
    {
        assertDerived<DataT, DerivedT>();
    }

    template <typename DerivedT>
    Handle& operator=(Handle<DerivedT>&& handle) noexcept {
        assertDerived<DataT, DerivedT>();
        AnyHandle::operator =(std::move(handle));
        return *this;
    }

    Handle(std::nullptr_t) noexcept
    {}

    Handle(const Handle& handle) noexcept
        : Handle<DataT>(handle, 1)
    {}

    Handle& operator=(const Handle& handle) noexcept {
        return operator=<DataT>(handle);
    }

    Handle(Handle&& handle) noexcept
        : Handle<DataT>(std::move(handle), 1)
    {}

    Handle& operator=(Handle&& handle) noexcept {
        return operator=<DataT>(std::move(handle));
    }

    bool isValid() noexcept {
        return m_strongHandle != nullptr && m_strongHandle->isValid();
    }

    DataT* operator->() const {
        if (m_strongHandle == nullptr)
            throw FlappyException("Invalid handle");
        return static_cast<const StrongHandle<DataT>*>(m_strongHandle)->operator->();
    }
};
