#pragma once

#include <cstddef>
#include <cstdint>

#include <Utility.hpp>

class Chank;
class AnyStrongHandle;

template <typename T>
class Handle;

class AnyHandle {
    friend class Chank; // to access invalidate() and setNewHandle()
public:
    AnyHandle() = default;

    AnyHandle(AnyStrongHandle& strongHandle) noexcept;

    AnyHandle& operator=(AnyStrongHandle& strongHandle) noexcept;

    AnyHandle(std::nullptr_t) noexcept
    {}

    template <typename DerivedT>
    AnyHandle(const Handle<DerivedT>& handle) noexcept
        : m_chank(handle.m_chank)
    {
        handle.m_chank->registerHandle(this);
    }

    template <typename DerivedT>
    AnyHandle& operator=(const Handle<DerivedT>& handle) noexcept {
        setNewChank(handle.m_chank);
        return *this;
    }

    template <typename DerivedT>
    AnyHandle(Handle<DerivedT>&& handle) noexcept
        : AnyHandle(handle) // explicit call copy constructor
    {
        handle.invalidate();
    }

    template <typename DerivedT>
    AnyHandle& operator=(Handle<DerivedT>&& handle) noexcept {
        operator=(handle); // explicit call assignment operator
        handle.invalidate();
        return *this;
    }

    // Not virtual because the only derived class has no data members
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
    Chank* chank() const noexcept { return m_chank; }

private:
    Chank* m_chank = nullptr;

    void setNewChank(Chank* chank) noexcept;
    void invalidate() noexcept;
};

static_assert(!std::is_polymorphic<AnyHandle>(), "AnyHandle should not be a polymorphic!");
static_assert(sizeof(AnyHandle) == sizeof(intptr_t), "AnyHandle should consist of a single pointer.");
