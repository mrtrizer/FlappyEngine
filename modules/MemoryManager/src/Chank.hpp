#pragma once

#include <memory>
#include <cstddef>

#include "StrongHandle.hpp"

/// The class holds object of any type within size limit
class Chank {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class ObjectPool;
    friend class std::allocator<Chank>;
    template <typename DataT>
    friend class StrongHandle;

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

    Chank(std::byte* data, size_t size)
        : m_data(data)
        , m_size(size)
    {}

    Chank(Chank&& other) noexcept {
        if (other.m_chankFunctions != nullptr) {
            other.m_chankFunctions->updateHandle(other.m_strongHandle, this, other.m_data);
        }
        m_data = other.m_data;
        m_size = other.m_size;
        m_strongHandle = other.m_strongHandle;
        m_chankFunctions = other.m_chankFunctions;
        m_destroyedCallback = other.m_destroyedCallback;
    }

    ~Chank() {
        if (m_chankFunctions != nullptr) {
            m_chankFunctions->destroy(m_data);
            m_destroyedCallback(this);
        }
    }

    void moveFrom(Chank* chank) {
        DEBUG_ASSERT(chank->m_size == m_size);
        DEBUG_ASSERT(chank->m_data != m_data);

        chank->m_chankFunctions->move(chank->m_data, m_data);
        chank->m_chankFunctions->updateHandle(chank->m_strongHandle, this, m_data);

        m_strongHandle = chank->m_strongHandle;
        m_chankFunctions = chank->m_chankFunctions;
        m_destroyedCallback = chank->m_destroyedCallback;

        chank->clear();
    }

    /// Instantiates object in chank and returns a strong handle. Underlying instance exists until the strong handle is destroyed.
    /// @param destroyedCallback Called when underlying class is destroyed
    template <typename DataT, typename ... Args>
    [[nodiscard]] StrongHandle<DataT> construct(std::function<void(Chank*)> destroyedCallback, Args&& ... args) {
        DEBUG_ASSERT(destroyedCallback != nullptr);
        DEBUG_ASSERT(m_strongHandle == nullptr);
        DEBUG_ASSERT(m_chankFunctions == nullptr);
        DEBUG_ASSERT(m_destroyedCallback == nullptr);

        static_assert(noexcept(DataT(std::declval<DataT>())), "DataT(DataT&&) should be noexcept.");

        try {
            auto data = new (m_data) DataT(std::forward<Args>(args)...);

            StrongHandle<DataT> strongHandle(data, this);

            // This pointer is automatically updated if the strong handle is moved to a new location
            m_strongHandle = &strongHandle;
            m_chankFunctions = chankFunctionsForType<DataT>();
            m_destroyedCallback = destroyedCallback;

            return strongHandle;
        } catch(...) {
            m_strongHandle = nullptr;
            m_chankFunctions = nullptr;
            m_destroyedCallback = nullptr;
            throw;
        }
    }

    /// Method destroys underlaying instance if it is was initialized.
    void clear() noexcept {
        if (constructed()) {
            DEBUG_ASSERT(m_strongHandle != nullptr);
            DEBUG_ASSERT(m_destroyedCallback != nullptr);

            m_chankFunctions->destroy(m_data);
            auto destroyedCallback = m_destroyedCallback;

            m_strongHandle = nullptr;
            m_chankFunctions = nullptr;
            m_destroyedCallback = nullptr;

            destroyedCallback(this);
        }
    }

    [[nodiscard]] bool constructed() const noexcept {
        return m_chankFunctions != nullptr;
    }

    template <typename DataT>
    DataT* data() noexcept {
        DEBUG_ASSERT(m_chankFunctions == chankFunctionsForType<DataT>());

        return reinterpret_cast<DataT*>(m_data);
    }

    std::byte* m_data = nullptr;
    size_t m_size;
    void* m_strongHandle = nullptr; // I prefer manual casting instead of introducing an interface
    IChankFunctions* m_chankFunctions = nullptr;
    std::function<void(Chank*)> m_destroyedCallback;
};
