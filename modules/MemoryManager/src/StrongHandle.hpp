#pragma once

#include "AnyStrongHandle.hpp"

template <typename DataT>
class Handle;

template <typename DataT>
class StrongHandle : public AnyStrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class Chank; // to construct
    template<typename T>
    friend class ChankFunctions; // to update pointer
public:
    StrongHandle& operator=(std::nullptr_t) noexcept {
        reset();
        return *this;
    }

    template <typename DerivedT>
    StrongHandle(StrongHandle<DerivedT>&& strongHandle) noexcept
        : AnyStrongHandle(std::move(strongHandle))
    {
        checkType<DerivedT>();
    }

    template <typename DerivedT>
    StrongHandle& operator=(StrongHandle<DerivedT>&& strongHandle) noexcept {
        checkType<DerivedT>();
        moveFromStrongHandle(std::move(strongHandle));
        return *this;
    }

    StrongHandle(StrongHandle&& strongHandle) noexcept
        : AnyStrongHandle(std::move(strongHandle))
    {}

    StrongHandle& operator=(StrongHandle&& strongHandle) noexcept {
        moveFromStrongHandle(std::move(strongHandle));
        return *this;
    }

    StrongHandle(const StrongHandle&) = delete;
    StrongHandle& operator=(const StrongHandle&) = delete;

    Handle<DataT> handle() noexcept {
        return Handle<DataT>(*this);
    }

    DataT* operator->() {
        if (!isValid())
            throw FlappyException("Invalid handle");
        return static_cast<DataT*>(m_dataPointer);
    }

private:
    StrongHandle(DataT* dataPointer,
                 Chank* chank) noexcept
        : AnyStrongHandle(getTypeId<DataT>(), dataPointer, chank)
    {}

    template <typename DerivedT>
    constexpr void checkType() {
        static_assert(std::is_base_of<DataT, DerivedT>::value, "DerivedT should be derived from BaseT");
    }
};
