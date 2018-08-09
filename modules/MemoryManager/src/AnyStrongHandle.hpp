#pragma once

#include <list>

#include "Utility.hpp"

class Chank;

template <typename DataT>
class StrongHandle;

struct HandleCalls {
    HandleCalls(void* rawPointer_,
                std::function<void(void)> invalidate_,
                std::function<void(void* strongHandle)> updateStrongHandle_)
        : rawPointer(rawPointer_)
        , invalidate(invalidate_)
        , updateStrongHandle(updateStrongHandle_)
    {}
    void* rawPointer = nullptr;
    std::function<void(void)> invalidate;
    std::function<void(void* strongHandle)> updateStrongHandle;
};

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
    std::list<HandleCalls> m_handles;

    AnyStrongHandle(TypeId typeId = 0,
                     void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept;

    void reset() noexcept;

    template <typename T>
    void registerHandle(T* handle) noexcept {
        DEBUG_ASSERT(handle != nullptr);

       m_handles.emplace_back(
           handle,
           std::bind(&T::invalidate, handle),
           std::bind(&T::updateStrongHandle, handle, std::placeholders::_1)
       );
    }

    void unregisterHandle(void* handle) noexcept;

    void moveFromStrongHandle(AnyStrongHandle&& strongHandle);

    void updatePointer(void* dataPointer, Chank* chank) noexcept;
};

static_assert(!std::is_polymorphic<AnyStrongHandle>(), "AnyStrongHandle should not be a polymorphic!");
