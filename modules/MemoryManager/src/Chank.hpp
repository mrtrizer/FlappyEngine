#pragma once

#include <memory>
#include <cstddef>

#include "StrongHandle.hpp"

template<size_t ChankSize>
class Chank;

template<size_t ChankSize>
class IChankFunctions {
public:
    virtual void destroy(std::byte* data) = 0;
    virtual void updateHandle(void* handle, Chank<ChankSize>* chank) = 0;
    virtual void move(std::byte* source, std::byte* destination) = 0;
};

template<typename DataT, size_t ChankSize>
class ChankFunctions : public IChankFunctions<ChankSize> {
public:
    void destroy(std::byte* data) override {
        reinterpret_cast<DataT*>(data)->~DataT();
    }

    void updateHandle(void* strongHandle, Chank<ChankSize>* chank) override;

    void move(std::byte* source, std::byte* destination) override;

    static IChankFunctions<ChankSize>& instance() {
        static ChankFunctions<DataT, ChankSize> staticInstance;
        return staticInstance;
    }
};

template<size_t ChankSize>
class Chank {
public:
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

    template <typename DataT, typename ... Args>
    StrongHandle<DataT> construct(std::function<void(Chank*)> destroyedCallback, Args&& ... args) {
        DEBUG_ASSERT(destroyedCallback != nullptr);
        DEBUG_ASSERT(m_strongHandle == nullptr);
        DEBUG_ASSERT(m_chankFunctions == nullptr);
        DEBUG_ASSERT(m_destroyedCallback == nullptr);

        static_assert(noexcept(DataT(std::declval<DataT>())), "DataT(DataT&&) should be noexcept.");

        try {
            auto data = new (m_data) DataT(std::forward<Args>(args)...);

            StrongHandle<DataT> strongHandle(data,
                                             [this] { clear(); },
                                             [this] (StrongHandle<DataT>* strongHandle) { m_chankFunctions->updateHandle(strongHandle, this); });

            m_strongHandle = &strongHandle;
            m_chankFunctions = &ChankFunctions<DataT, ChankSize>::instance();
            m_destroyedCallback = destroyedCallback;

            return strongHandle;
        } catch(...) {
            m_strongHandle = nullptr;
            m_chankFunctions = nullptr;
            m_destroyedCallback = nullptr;
            throw;
        }
    }

    bool cunstructed() {
        return m_chankFunctions != nullptr;
    }

    void clear() noexcept {
        DEBUG_ASSERT(m_strongHandle != nullptr);
        DEBUG_ASSERT(m_chankFunctions != nullptr);
        DEBUG_ASSERT(m_destroyedCallback != nullptr);

        m_chankFunctions->destroy(m_data);
        m_strongHandle = nullptr;
        m_chankFunctions = nullptr;
        m_destroyedCallback = nullptr;
    }

    template <typename DataT>
    DataT* data() noexcept {
        DEBUG_ASSERT(m_chankFunctions == &(ChankFunctions<DataT, ChankSize>::instance()));

        return reinterpret_cast<DataT*>(m_data);
    }

private:
    std::byte m_data[ChankSize];
    void* m_strongHandle = nullptr;
    IChankFunctions<ChankSize>* m_chankFunctions;
    std::function<void(Chank*)> m_destroyedCallback;
};

template<typename DataT, size_t ChankSize>
void ChankFunctions<DataT, ChankSize>::updateHandle(void* strongHandle, Chank<ChankSize>* chank) {
    reinterpret_cast<StrongHandle<DataT>*>(strongHandle)->updatePointer(
                chank->template data<DataT>(),
                [this, chank] { chank->clear(); },
                [this, chank] (StrongHandle<DataT>* strongHandle) { updateHandle(strongHandle, chank); }
    );
}

template<typename DataT, size_t ChankSize>
void ChankFunctions<DataT, ChankSize>::move(std::byte* source, std::byte* destination) {
    new (destination) DataT(std::move(*reinterpret_cast<DataT*>(source)));
}
