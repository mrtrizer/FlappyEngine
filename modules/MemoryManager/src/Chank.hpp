#pragma once

#include <memory>
#include <cstddef>

#include "StrongHandle.hpp"

class ObjectPool;

/// The class holds object of any type within size limit
class Chank {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class ObjectPool; // to create instance and call methods
    friend class std::allocator<Chank>; // to move instance in vector reallocation
    template <typename DataT>
    friend class StrongHandle; // to update handle
    friend class AnyStrongHandle; // to update handle

    /// The interface generalises work with different data types with minimal overhead
    class IChankFunctions {
    public:
        virtual void destroy(std::byte* data) = 0;
        virtual void updateHandle(void* handle, Chank* chank, std::byte* data) = 0;
        virtual void move(std::byte* source, std::byte* destination) = 0;
    };

    template<typename DataT>
    class ChankFunctions : public IChankFunctions {
    public:
        void destroy(std::byte* data) override {
            reinterpret_cast<DataT*>(data)->~DataT();
        }

        void updateHandle(void* strongHandle, Chank* chank, std::byte* data) override {
            reinterpret_cast<StrongHandle<DataT>*>(strongHandle)->updatePointer(
                reinterpret_cast<DataT*>(data), chank
            );
        }

        void move(std::byte* source, std::byte* destination) override {
            new (destination) DataT(std::move(*reinterpret_cast<DataT*>(source)));
        }
    };

    template <typename DataT>
    static IChankFunctions* chankFunctionsForType() {
        static ChankFunctions<DataT> instance;
        return &instance;
    }

    Chank(std::byte* data, size_t size);
    Chank(Chank&& other) noexcept;
    ~Chank();

    void moveFrom(Chank* chank);

    /// Instantiates object in chank and returns a strong handle. Underlying instance exists until the strong handle is destroyed.
    /// @param destroyedCallback Called when underlying class is destroyed
    template <typename DataT, typename ... Args>
    [[nodiscard]] StrongHandle<DataT> construct(ObjectPool* objectPool, Args&& ... args) {
        DEBUG_ASSERT(m_strongHandle == nullptr);
        DEBUG_ASSERT(m_chankFunctions == nullptr);
        DEBUG_ASSERT(m_objectPool == nullptr);

        static_assert(noexcept(DataT(std::declval<DataT>())), "DataT(DataT&&) should be noexcept.");

        try {
            auto data = new (m_data) DataT(std::forward<Args>(args)...);

            StrongHandle<DataT> strongHandle(data, this);

            // This pointer is automatically updated if the strong handle is moved to a new location
            m_strongHandle = &strongHandle;
            m_chankFunctions = chankFunctionsForType<DataT>();
            m_objectPool = objectPool;

            return strongHandle;
        } catch(...) {
            m_strongHandle = nullptr;
            m_chankFunctions = nullptr;
            m_objectPool = nullptr;
            throw;
        }
    }

    /// Method destroys underlaying instance if it is was initialized.
    void clear() noexcept;

    bool empty() const noexcept { return m_strongHandle == nullptr; }

    [[nodiscard]] bool constructed() const noexcept;

    template <typename DataT>
    DataT* data() noexcept {
        DEBUG_ASSERT(m_chankFunctions == chankFunctionsForType<DataT>());
        return reinterpret_cast<DataT*>(m_data);
    }

    std::byte* m_data = nullptr;
    size_t m_size;
    void* m_strongHandle = nullptr; // I prefer manual casting instead of introducing an interface
    IChankFunctions* m_chankFunctions = nullptr;
    ObjectPool* m_objectPool = nullptr;
};
