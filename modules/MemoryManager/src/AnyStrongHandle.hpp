#pragma once

#include <list>

#include <ObjectId.hpp>

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
    friend bool operator==(const AnyStrongHandle&, const AnyStrongHandle&);
    friend bool operator==(const AnyStrongHandle&, const AnyHandle&);
    friend bool operator==(const AnyHandle&, const AnyStrongHandle&);
    friend struct std::hash<AnyStrongHandle>;
public:
    AnyStrongHandle(std::nullptr_t) noexcept {}

    AnyStrongHandle& operator=(std::nullptr_t) noexcept;

    AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept;

    AnyStrongHandle& operator=(AnyStrongHandle&& strongHandle) noexcept;

    // Not virtual because the only derived class has no data members
    ~AnyStrongHandle() noexcept;

    bool isValid() const noexcept;

    TypeId typeId() const noexcept;
    
    ObjectId objectId() const noexcept;

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

inline bool operator==(const AnyStrongHandle& a, const AnyStrongHandle& b) { return a.m_chank == b.m_chank; }

inline bool operator!=(const AnyStrongHandle& a, const AnyStrongHandle& b) { return !operator==(a, b); }

} // flappy

namespace std {

template <>
struct hash<flappy::AnyStrongHandle>
{
    std::size_t operator()(const flappy::AnyStrongHandle& k) const
    {
      return std::hash<decltype(k.m_chank)>()(k.m_chank);
    }
};

} // std
