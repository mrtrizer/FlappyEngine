#pragma once

#include <list>

#include "Utility.hpp"

class Chank;
class AnyHandle;

template <typename DataT>
class StrongHandle;

class AnyStrongHandle {
    template <typename T>
    friend class Handle; // to register/unregister handles
    friend class AnyHandle; // to register/unregister handles
public:
    AnyStrongHandle& operator=(std::nullptr_t) noexcept;

    AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept;

    AnyStrongHandle& operator=(AnyStrongHandle&& strongHandle) noexcept;

    // Destructor should not be virtual
    ~AnyStrongHandle();

    bool isValid() const noexcept;

    TypeId typeId() const noexcept;

protected:
    TypeId m_typeId;
    void* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;
    std::list<AnyHandle*> m_handles;

    AnyStrongHandle(TypeId typeId = 0,
                     void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept;

    void reset() noexcept;

    void registerHandle(AnyHandle* handle) noexcept;

    void unregisterHandle(void* handle) noexcept;

    void moveFromStrongHandle(AnyStrongHandle&& strongHandle);

    void updatePointer(void* dataPointer, Chank* chank) noexcept;
};

static_assert(!std::is_polymorphic<AnyStrongHandle>(), "AnyStrongHandle should not be a polymorphic!");
