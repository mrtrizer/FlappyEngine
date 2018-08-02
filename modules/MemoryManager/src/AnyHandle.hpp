#pragma once

#include "AnyStrongHandle.hpp"

template <typename T>
class Handle;

class AnyHandle {
    friend class AnyStrongHandle; // to access invalidate() and setNewHandle()
public:
    AnyHandle() = default;

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
    AnyHandle(const Handle<DerivedT>& handle) noexcept
        : m_strongHandle(handle.m_strongHandle)
    {
        m_strongHandle->registerHandle(this);
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
    ~AnyHandle() {
        if (m_strongHandle != nullptr)
            m_strongHandle->unregisterHandle(this);
    }

    bool isValid() noexcept {
        return m_strongHandle != nullptr && m_strongHandle->isValid();
    }

    template <typename DataT>
    const Handle<DataT>& get() const {
        if (m_strongHandle->typeId() == getTypeId<DataT>())
            return *static_cast<const Handle<DataT>*>(this);
        else
            throw FlappyException("AnyHandle points to another type.");
    }

protected:
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

    AnyStrongHandle* m_strongHandle = nullptr;
};

static_assert(!std::is_polymorphic<AnyHandle>(), "AnyHandle should not be a polymorphic!");
static_assert(sizeof(AnyHandle) == sizeof(intptr_t), "AnyHandle should consist of a single pointer.");
