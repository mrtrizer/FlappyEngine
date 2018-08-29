#pragma once

#include "AnyStrongHandle.hpp"

namespace flappy {

template <typename DataT>
class Handle;

template <typename DataT>
class StrongHandle : public AnyStrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger); // to access handles and a chank
    friend class Chank; // to construct
public:
    StrongHandle& operator=(std::nullptr_t) noexcept {
        AnyStrongHandle::operator=(nullptr);
        return *this;
    }

    template <typename DerivedT>
    StrongHandle(StrongHandle<DerivedT>&& strongHandle) noexcept
        : AnyStrongHandle(std::move(strongHandle))
    {
        assertDerived<DataT, DerivedT>();
    }

    template <typename DerivedT>
    StrongHandle& operator=(StrongHandle<DerivedT>&& strongHandle) noexcept {
        assertDerived<DataT, DerivedT>();
        AnyStrongHandle::operator=(std::move(strongHandle));
        return *this;
    }

    /// A duplicate of a copy constructor to support an implicit construction
    StrongHandle(StrongHandle&& strongHandle) noexcept
        : AnyStrongHandle(std::move(strongHandle))
    {}

    /// A duplicate of operator= to support an implicit construction
    StrongHandle& operator=(StrongHandle&& strongHandle) noexcept {
        AnyStrongHandle::operator=(std::move(strongHandle));
        return *this;
    }

    StrongHandle(const StrongHandle&) = delete;
    StrongHandle& operator=(const StrongHandle&) = delete;

    Handle<DataT> handle() noexcept {
        return Handle<DataT>(*this);
    }

    DataT* operator->() const {
        if (!isValid())
            throw FlappyException("Invalid handle");
        return static_cast<DataT*>(dataPointer());
    }

private:
    /// Strong handle can't be constructed outside ObjectPool
    StrongHandle(DataT* dataPointer,
                 Chank* chank) noexcept
        : AnyStrongHandle(dataPointer, chank)
    {}
};

} // flappy
