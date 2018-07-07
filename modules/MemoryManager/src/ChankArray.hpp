#pragma once

#include <memory>

#include "StrongHandle.hpp"

template <typename DataT>
class Chank {
public:
    Chank(DataT&& data)
        : m_data(std::move(data))
    {}

    void setStrongHandle(StrongHandle<DataT>* handle) noexcept { m_strongHandle = handle; }
    StrongHandle<DataT>* strongHandle() noexcept { return m_strongHandle; }

    DataT& data() noexcept { return m_data; }

private:
    StrongHandle<DataT>* m_strongHandle = nullptr;
    DataT m_data;
};

template <typename DataT>
class ChankArray {
    using Allocator = std::allocator<Chank<DataT>>;
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
            AllocatorTraits::destroy(m_allocator, pointer);
        AllocatorTraits::deallocate(m_allocator, m_first, m_capacity);
    }

    template <typename...Args>
    StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ChankArray doesn't support basic types.");

        if (m_length >= m_capacity)
            throw FlappyException(sstr("You have reached limit of chanks. Max: ", m_capacity));

        auto chank = m_end;
        AllocatorTraits::construct(m_allocator, chank, DataT(std::forward<Args>(args)...));
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
    Chank<DataT>* m_first = nullptr;
    Chank<DataT>* m_end = nullptr;

    void updatePointer(StrongHandle<DataT>* strongHandle, const Chank<DataT>* chank) {
        DEBUG_ASSERT(strongHandle != nullptr);
        DEBUG_ASSERT(chank != nullptr);

        strongHandle->updatePointer(
                    chank->data(),
                    [this, chank] { remove(chank); },
                    [this, chank] (StrongHandle<DataT>* newStrongHandle) { chank->setStrongHandle(newStrongHandle); }
        );
    }

    void remove(Chank<DataT>* chank) {
        DEBUG_ASSERT(m_length > 0);
        DEBUG_ASSERT(m_end != nullptr);

        AllocatorTraits::destroy(m_allocator, chank);
        auto last = m_end - 1;
        if (m_length > 1 && chank != std::prev(m_end))
            AllocatorTraits::construct(m_allocator, chank, std::move(*last));
        AllocatorTraits::destroy(m_allocator, last);
        --m_length;
        --m_end;
    }
};
