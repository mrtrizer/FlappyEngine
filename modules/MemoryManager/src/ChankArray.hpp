#pragma once

#include <memory>
#include <cstddef>

#include "StrongHandle.hpp"

// The class is needed to destroy list of chanks on ChankArray destruction
class IChank {
public:
    virtual ~IChank() = default;
};

template <typename DataT>
class Chank : public IChank {
public:
    Chank(DataT&& data)
        : m_data(std::move(data))
    {}

    template <typename ... Args>
    static Chank<DataT>* construct(void* desintation, Args ... args) {
        return new (desintation) Chank(std::forward<Args>(args)...);
    }

    void setStrongHandle(StrongHandle<DataT>* handle) noexcept { m_strongHandle = handle; }
    StrongHandle<DataT>* strongHandle() noexcept { return m_strongHandle; }

    DataT& data() noexcept { return m_data; }

private:
    StrongHandle<DataT>* m_strongHandle = nullptr;
    DataT m_data;
};

template<size_t ChankSize>
class ChankArray {
    using ChankBytes = std::byte[ChankSize];
    using Allocator = std::allocator<ChankBytes>;
    using AllocatorTraits = std::allocator_traits<Allocator>;
public:
    ChankArray(size_t capacity)
        : m_capacity(capacity)
    {
        USER_ASSERT(capacity > 0);

        m_end = m_first = AllocatorTraits::allocate(m_allocator, capacity);
    }
    ChankArray(const ChankArray&) = delete;
    ChankArray& operator= (const ChankArray&) = delete;
    ChankArray(ChankArray&& chankArray) = delete;
    ChankArray& operator= (ChankArray&&) = default;
    ~ChankArray() {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        for (auto pointer = m_first; pointer != m_end; ++pointer)
            reinterpret_cast<IChank*>(pointer)->~IChank();
        AllocatorTraits::deallocate(m_allocator, m_first, m_capacity);
    }

    template <typename DataT, typename...Args>
    StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ChankArray doesn't support basic types.");

        if (m_length >= m_capacity)
            throw FlappyException(sstr("You have reached limit of chanks. Max: ", m_capacity));

        auto chank = Chank<DataT>::construct(m_end, DataT(std::forward<Args>(args)...));
        StrongHandle<DataT> strongHandle(&chank->data(),
                                         [this, chank] { remove(chank); },
                                         [this, chank] (StrongHandle<DataT>* newStrongHandle) { chank->setStrongHandle(newStrongHandle); });
        chank->setStrongHandle(&strongHandle);
        ++m_end;
        ++m_length;
        return strongHandle;
    }

private:
    size_t m_capacity = 0;
    size_t m_length = 0;
    Allocator m_allocator;
    ChankBytes* m_first = nullptr;
    ChankBytes* m_end = nullptr;

    template <typename DataT>
    void updatePointer(StrongHandle<DataT>* strongHandle, Chank<DataT>* chank) {
        DEBUG_ASSERT(strongHandle != nullptr);
        DEBUG_ASSERT(chank != nullptr);

        strongHandle->updatePointer(
                    &chank->data(),
                    [this, chank] { remove(chank); },
                    [this, chank] (StrongHandle<DataT>* newStrongHandle) { chank->setStrongHandle(newStrongHandle); }
        );
    }

    template <typename DataT>
    void remove(Chank<DataT>* chank) {
        DEBUG_ASSERT(m_length > 0);
        DEBUG_ASSERT(m_end != nullptr);

        reinterpret_cast<IChank*>(chank)->~IChank();
        auto last = m_end - 1;
        if (m_length > 1 && reinterpret_cast<ChankBytes*>(chank) != std::prev(m_end)) {
            Chank<DataT>::construct(chank, std::move(*reinterpret_cast<Chank<DataT>*>(last)));
            updatePointer(chank->strongHandle(), chank);
            reinterpret_cast<IChank*>(last)->~IChank();
        }
        --m_length;
        --m_end;
    }
};
