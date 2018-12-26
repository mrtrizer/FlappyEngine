#pragma once

#include <cstddef>
#include <cstdint>

#include <Utility.hpp>
#include <TypeId.hpp>

namespace flappy {

class Chank;
class AnyStrongHandle;

template <typename T>
class Handle;

class AnyHandle {
    friend class Chank; // to access invalidate() and setNewHandle()
    friend bool operator==(const AnyStrongHandle&, const AnyHandle&);
    friend bool operator==(const AnyHandle&, const AnyStrongHandle&);
    friend bool operator==(const AnyHandle&, const AnyHandle&);
public:
    AnyHandle() = default;

    AnyHandle(Chank* chank) noexcept;

    AnyHandle(const AnyStrongHandle& strongHandle) noexcept;

    AnyHandle& operator=(const AnyStrongHandle& strongHandle) noexcept;

    AnyHandle(std::nullptr_t) noexcept
    {}

    template <typename DerivedT>
    AnyHandle(const Handle<DerivedT>& handle) noexcept
        : m_chank(nullptr)
    {
        setNewChank(handle.m_chank);
    }

    template <typename DerivedT>
    AnyHandle& operator=(const Handle<DerivedT>& handle) noexcept {
        if (&handle != this)
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
        operator=(handle); // explicit call copy assignment operator
        handle.invalidate();
        return *this;
    }

    // Not virtual because the only derived class has no data members
    ~AnyHandle();

    bool isValid() const noexcept;

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
    
    bool isSame(const AnyStrongHandle& strongHandle) const;

    void setNewChank(Chank* chank) noexcept;
    void invalidate() noexcept;
};

static_assert(!std::is_polymorphic<AnyHandle>(), "AnyHandle should not be a polymorphic!");
static_assert(sizeof(AnyHandle) == sizeof(intptr_t), "AnyHandle should consist of a single pointer.");

inline bool operator==(const AnyHandle& a, std::nullptr_t) { return !a.isValid(); }
inline bool operator==(std::nullptr_t, const AnyHandle& a) { return !a.isValid(); }

inline bool operator!=(const AnyHandle& a, std::nullptr_t) { return a.isValid(); }
inline bool operator!=(std::nullptr_t, const AnyHandle& a) { return a.isValid(); }

inline bool operator==(const AnyHandle& a, const AnyHandle& b) { return a.m_chank == b.m_chank; }

inline bool operator!=(const AnyHandle& a, const AnyHandle& b) { return !operator==(a, b); }

bool operator==(const AnyStrongHandle& a, const AnyHandle& b);

bool operator==(const AnyHandle& a, const AnyStrongHandle& b);

} // flappy
