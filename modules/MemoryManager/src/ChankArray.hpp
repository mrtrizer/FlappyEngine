#pragma once

#include "Chank.hpp"

template<size_t ChankSize>
class ChankArray {
    using Allocator = std::allocator<Chank<ChankSize>>;
    using AllocatorTraits = std::allocator_traits<Allocator>;
public:
    ChankArray(std::size_t capacity)
        : m_capacity(capacity)
    {
        USER_ASSERT(capacity > 0);

        auto array = m_end = m_first = AllocatorTraits::allocate(m_allocator, capacity);
        for (int i = 0; i < m_capacity; ++i)
            AllocatorTraits::construct(m_allocator, &array[i]);
    }
    ChankArray(const ChankArray&) = delete;
    ChankArray& operator= (const ChankArray&) = delete;
    ChankArray(ChankArray&& chankArray) = delete;
    ChankArray& operator= (ChankArray&&) = default;
    ~ChankArray() {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        for (int i = 0; i < m_capacity; ++i)
            AllocatorTraits::destroy(m_allocator, &m_first[i]);
        AllocatorTraits::deallocate(m_allocator, m_first, m_capacity);
    }

    template <typename DataT, typename...Args>
    StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ChankArray doesn't support basic types.");
        static_assert(sizeof(DataT) <= ChankSize, "DataT doesn't fit into a chank of size ChankSize.");

        if (m_length >= m_capacity)
            throw FlappyException(sstr("You have reached limit of chanks. Max: ", m_capacity));

        auto onDestroyed = [this](Chank<ChankSize>* chank) {
            // if the chank is not the last element, replace it with the last element
            auto last = m_end - 1;
            if (chank != last)
                *chank = std::move(*last);
            m_end--;
            m_length--;
        };

        auto strongHandle =  m_end->template construct<DataT>(onDestroyed, std::forward<Args>(args)...);

        m_end++;
        m_length++;

        return strongHandle;
    }

private:
    size_t m_capacity = 0;
    size_t m_length = 0;
    Allocator m_allocator;
    Chank<ChankSize>* m_first = nullptr;
    Chank<ChankSize>* m_end = nullptr;
};
