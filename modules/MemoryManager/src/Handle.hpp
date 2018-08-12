#pragma once

#include <iostream>
#include <memory>

#include "StrongHandle.hpp"
#include "AnyHandle.hpp"

template <typename DataT>
class Handle : public AnyHandle {
    template <typename T>
    friend class Handle; // for interided type convertion
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

    DataT* operator->() const {
        if (chank() == nullptr)
            throw FlappyException("Invalid handle");
        return chank()->template data<DataT>();
    }
};
