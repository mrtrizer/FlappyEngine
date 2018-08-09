#pragma once

#include <cstddef>
#include <cstdint>

#include "Utility.hpp"

class AnyStrongHandle;

template <typename T>
class Handle;

class AnyHandle {
    friend class AnyStrongHandle; // to access invalidate() and setNewHandle()
public:
    AnyHandle() = default;

    AnyHandle(AnyStrongHandle& strongHandle) noexcept;

    AnyHandle& operator=(AnyStrongHandle& strongHandle) noexcept;

    AnyHandle(std::nullptr_t) noexcept
    {}

    template <typename DerivedT>
    AnyHandle(const Handle<DerivedT>& handle) noexcept
        : m_strongHandle(handle.m_strongHandle)
    {
        registerInStrongHandle();
    }

    template <typename DerivedT>
    AnyHandle& operator=(const Handle<DerivedT>& handle) noexcept {
        setNewHandle(handle.m_strongHandle);
        return *this;
    }

    template <typename DerivedT>
    AnyHandle(Handle<DerivedT>&& handle) noexcept
        : AnyHandle(handle) // explicit call copy constructor
    {
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
    }

    template <typename DerivedT>
    AnyHandle& operator=(Handle<DerivedT>&& handle) noexcept{
        operator=(handle); // explicit call assignment operator
        handle.m_strongHandle->unregisterHandle(&handle);
        handle.invalidate();
        return *this;
    }

    // Destructor should not be virtual in this case
    ~AnyHandle();

    bool isValid() noexcept;

    template <typename DataT>
    const Handle<DataT>& get() const {
        if (typeId() == getTypeId<DataT>())
            return *static_cast<const Handle<DataT>*>(this);
        else
            throw FlappyException("AnyHandle points to another type.");
    }

    TypeId typeId() const noexcept;

protected:
    void setNewHandle(AnyStrongHandle* strongHandle) noexcept;
    void invalidate() noexcept;
    // strongHandlePtr is void* to support anonymous handles
    void updateStrongHandle(void* strongHandlePtr) noexcept;
    void registerInStrongHandle() noexcept;

    AnyStrongHandle* m_strongHandle = nullptr;
};

static_assert(!std::is_polymorphic<AnyHandle>(), "AnyHandle should not be a polymorphic!");
static_assert(sizeof(AnyHandle) == sizeof(intptr_t), "AnyHandle should consist of a single pointer.");
