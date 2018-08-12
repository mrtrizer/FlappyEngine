#pragma once

#include <list>

#include "Utility.hpp"

class Chank;
class AnyHandle;

template <typename DataT>
class StrongHandle;

class AnyStrongHandle {
    FORDEBUG(friend class ObjectPoolDebugger); // for access debug information
    friend class AnyHandle; // to register/unregister handles
public:
    AnyStrongHandle& operator=(std::nullptr_t) noexcept;

    AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept;

    AnyStrongHandle& operator=(AnyStrongHandle&& strongHandle) noexcept;

    // Not virtual because the only derived class has no data members
    ~AnyStrongHandle();

    bool isValid() const noexcept;

    TypeId typeId() const noexcept;

protected:
    AnyStrongHandle(TypeId typeId = 0,
                     void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept;

    void* dataPointer() const noexcept { return m_dataPointer; }
    void updatePointer(void* dataPointer, Chank* chank) noexcept;

private:
    TypeId m_typeId;
    void* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;
    std::list<AnyHandle*> m_handles;

    void reset() noexcept;
    void registerHandle(AnyHandle* handle) noexcept;
    void unregisterHandle(void* handle) noexcept;
    void moveFromStrongHandle(AnyStrongHandle&& strongHandle);
};

static_assert(!std::is_polymorphic<AnyStrongHandle>(), "AnyStrongHandle should not be a polymorphic!");
