#pragma once

#include <list>

#include "Utility.hpp"
#include "TypeId.hpp"

namespace flappy {

class Chank;
class AnyHandle;

template <typename DataT>
class StrongHandle;

class AnyStrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger); // for access debug information
    friend class AnyHandle; // to register/unregister handles
    friend class Chank; // for access to updatePointer()
public:
    AnyStrongHandle(std::nullptr_t) noexcept {}

    AnyStrongHandle& operator=(std::nullptr_t) noexcept;

    AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept;

    AnyStrongHandle& operator=(AnyStrongHandle&& strongHandle) noexcept;

    // Not virtual because the only derived class has no data members
    ~AnyStrongHandle() noexcept;

    bool isValid() const noexcept;

    bool operator==(const AnyStrongHandle& other) const { return m_chank == other.m_chank; }

    bool operator!=(const AnyStrongHandle& other) const { return !operator==(other); }

    TypeId typeId() const noexcept;

protected:
    AnyStrongHandle(void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept;

    void* dataPointer() const noexcept { return m_dataPointer; }
    void updatePointer(void* dataPointer, Chank* chank) noexcept;

private:
    void* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;

    void reset() noexcept;
    void moveFromStrongHandle(AnyStrongHandle&& strongHandle) noexcept;
};

static_assert(!std::is_polymorphic<AnyStrongHandle>(), "AnyStrongHandle should not be a polymorphic!");

} // flappy
