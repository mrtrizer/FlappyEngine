#pragma once

#include <memory>
#include <cstddef>
#include <limits>

#include "StrongHandle.hpp"

namespace flappy {

class ObjectPool;

/// The class holds object of any type within size limit
class Chank {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class ObjectPool; // to create instance and call methods
    friend class AnyStrongHandle; // to update strong handle
    friend class AnyHandle; // to register/unregister handles
    template <typename T>
    friend class EnableSelfHandle; // to access m_strongHandle
    template <typename T>
    friend class Handle;

public:
    Chank(ObjectPool* objectPool, std::byte* data, size_t size);
    ~Chank();

private:
    /// Instantiates object in chank and returns a strong handle. Underlying instance exists until the strong handle is destroyed.
    /// @param destroyedCallback Called when underlying class is destroyed
    template <typename DataT, typename ... Args>
    [[nodiscard]] StrongHandle<DataT> construct(Args&& ... args) {
        DEBUG_ASSERT(m_strongHandle == nullptr);
        DEBUG_ASSERT(m_dataDescructor == nullptr);

//        static_assert(noexcept(DataT(std::declval<DataT>())), "DataT(DataT&&) should be noexcept.");

        try {
            // temporary value to make chank busy during construction
            auto data = new (m_data) DataT(std::forward<Args>(args)...);

            StrongHandle<DataT> strongHandle(data, this);

            // This pointer is automatically updated if the strong handle is moved to a new location
            m_strongHandle = &strongHandle;
            m_dataDescructor = [](void* data) { reinterpret_cast<DataT*>(data)->~DataT(); };
            m_typeId = getTypeId<DataT>();

            return strongHandle;
        } catch(...) {
            m_strongHandle = nullptr;
            m_dataDescructor = nullptr;
            // Self handles could be created during construction, so, removing them
            clearHandles();
            throw;
        }
    }

    /// Method destroys underlaying instance if it is was initialized.
    void clear() noexcept;

    bool empty() const noexcept { return m_strongHandle == nullptr; }

    TypeId typeId() const noexcept { return m_typeId; }

    [[nodiscard]] bool constructed() const noexcept;

    void registerHandle(AnyHandle* handle) noexcept;
    void unregisterHandle(void* handle) noexcept;
    void clearHandles() noexcept;

    template <typename DataT>
    DataT* data() noexcept {
        DEBUG_ASSERT(m_data != nullptr);
        return reinterpret_cast<DataT*>(m_data);
    }

    std::byte* m_data = nullptr;
    size_t m_size;
    AnyStrongHandle* m_strongHandle = nullptr;
    std::function<void(void*)> m_dataDescructor;
    ObjectPool* m_objectPool = nullptr;
    std::list<AnyHandle*> m_handles;
    TypeId m_typeId;
    bool m_hasRemovedHandles = false;
};

} // flappy
