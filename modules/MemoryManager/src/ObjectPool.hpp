#pragma once

#include "Chank.hpp"

template<size_t ChankSize>
class ObjectPool {
    FORDEBUG(friend class ObjectPoolDebugger);
    using Allocator = std::allocator<Chank<ChankSize>>;
    using AllocatorTraits = std::allocator_traits<Allocator>;
public:
    ObjectPool(std::size_t capacity)
        : m_capacity(capacity)
    {
        USER_ASSERT(capacity > 0);

        auto array = m_end = m_first = AllocatorTraits::allocate(m_allocator, capacity);
        for (int i = 0; i < m_capacity; ++i)
            AllocatorTraits::construct(m_allocator, &array[i]);
    }
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator= (const ObjectPool&) = delete;
    ObjectPool(ObjectPool&& chankArray) = delete;
    ObjectPool& operator= (ObjectPool&&) = default;
    ~ObjectPool() {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        for (int i = 0; i < m_capacity; ++i)
            AllocatorTraits::destroy(m_allocator, &m_first[i]);
        AllocatorTraits::deallocate(m_allocator, m_first, m_capacity);
    }

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_first != nullptr);
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ChankArray doesn't support basic types.");
        static_assert(sizeof(DataT) <= ChankSize, "DataT doesn't fit into a chank of size ChankSize.");

        USER_ASSERT_MSG(m_length < m_capacity, "You have reached limit of chanks. Max: ", m_capacity);

        auto end = m_end;
        auto length = m_length;

        try {
            auto strongHandle = end->template construct<DataT>(
                        std::bind(&ObjectPool::onDestroyed, this, std::placeholders::_1),
                        std::forward<Args>(args)...);
            m_end = end + 1;
            m_length = length + 1;
            return strongHandle;
        } catch (...) {
            m_end = end;
            m_length = length;
            throw;
        }
    }

private:
    size_t m_capacity = 0;
    size_t m_length = 0;
    Allocator m_allocator;
    Chank<ChankSize>* m_first = nullptr;
    Chank<ChankSize>* m_end = nullptr;

    void onDestroyed (Chank<ChankSize>* chank) noexcept {
        // if the chank is not the last element, replace it with the last element
        auto last = m_end - 1;
        if (chank != last)
            *chank = std::move(*last);
        m_end = last;
        m_length--;
    };
};