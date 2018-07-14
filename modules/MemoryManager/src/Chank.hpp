#pragma once

#include <memory>
#include <cstddef>

#include "StrongHandle.hpp"

template <size_t ChankSize>
class ChankArray;

/// The class holds object of any type within size limit
template<size_t ChankSize>
class Chank {
    friend class ChankArray<ChankSize>;
    friend class std::allocator<Chank<ChankSize>>;

    /// The interface generalises work with different data types with minimal overhead
    class IChankFunctions {
    public:
        virtual void destroy(std::byte* data) = 0;
        virtual void updateHandle(void* handle, Chank<ChankSize>* chank) = 0;
        virtual void move(std::byte* source, std::byte* destination) = 0;
    };

    template<typename DataT>
    class ChankFunctions : public IChankFunctions {
    public:
        void destroy(std::byte* data) override {
            reinterpret_cast<DataT*>(data)->~DataT();
        }

        void updateHandle(void* strongHandle, Chank<ChankSize>* chank) override {
            reinterpret_cast<StrongHandle<DataT>*>(strongHandle)->updatePointer(
                chank->template data<DataT>(),
                [chank] () noexcept { chank->clear(); },
                [this, chank] (StrongHandle<DataT>* strongHandle) noexcept { updateHandle(strongHandle, chank); }
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

    Chank() = default;

    Chank& operator=(Chank&& other) noexcept {
        // FIXME: Order dependant code. other.clear() should be called last
        other.m_chankFunctions->move(other.m_data, m_data);
        other.m_chankFunctions->updateHandle(other.m_strongHandle, this);
        m_strongHandle = other.m_strongHandle;
        m_chankFunctions = other.m_chankFunctions;
        m_destroyedCallback = other.m_destroyedCallback;
        other.clear();
        return *this;
    }

    ~Chank() {
        if (m_chankFunctions != nullptr)
            m_chankFunctions->destroy(m_data);
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

            StrongHandle<DataT> strongHandle(
                data,
                [this] () noexcept { clear(); },
                [this] (StrongHandle<DataT>* strongHandle) noexcept{ m_chankFunctions->updateHandle(strongHandle, this); });

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
        if (cunstructed()) {
            DEBUG_ASSERT(m_strongHandle != nullptr);
            DEBUG_ASSERT(m_destroyedCallback != nullptr);

            m_chankFunctions->destroy(m_data);
            m_strongHandle = nullptr;
            m_chankFunctions = nullptr;
            m_destroyedCallback = nullptr;
        }
    }

    [[nodiscard]] bool cunstructed() const noexcept {
        return m_chankFunctions != nullptr;
    }

    template <typename DataT>
    DataT* data() noexcept {
        DEBUG_ASSERT(m_chankFunctions == chankFunctionsForType<DataT>());

        return reinterpret_cast<DataT*>(m_data);
    }

    std::byte m_data[ChankSize];
    void* m_strongHandle = nullptr; // I prefer manual casting instead of introducing an interface
    IChankFunctions* m_chankFunctions;
    std::function<void(Chank*)> m_destroyedCallback;
};
