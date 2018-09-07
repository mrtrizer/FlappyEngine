#include "ObjectPool.hpp"

#include <memory>

namespace flappy {

ObjectPool::ObjectPool(size_t maxObjectSize, size_t capacity)
    : m_maxObjectSize(maxObjectSize)
    , m_bytes(capacity * maxObjectSize)
    , m_capacity(capacity)
{
    USER_ASSERT(capacity > 0);
    USER_ASSERT(maxObjectSize >= sizeof(int));

    m_chanks = std::allocator_traits<ChankAllocatorT>::allocate(m_chankAllocator, capacity);
    for (size_t i = 0; i < capacity; ++i)
        std::allocator_traits<ChankAllocatorT>::construct(m_chankAllocator, m_chanks + i, this, &m_bytes[maxObjectSize * i], maxObjectSize);
}

ObjectPool::~ObjectPool() noexcept {
    if (m_chanks == nullptr)
        return;
    for (size_t i = 0; i < m_capacity; ++i)
        std::allocator_traits<ChankAllocatorT>::destroy(m_chankAllocator, m_chanks + i);
    std::allocator_traits<ChankAllocatorT>::deallocate(m_chankAllocator, m_chanks, m_capacity);
}

void ObjectPool::onDestroyed (Chank* chank) noexcept {
    DEBUG_ASSERT(m_length > 0); // 1
    if (chank == &m_chanks[m_length - 1]) { // 1 - 1
        do
            --m_length; // 1 - 1 = 0
        while (m_length > 0 && m_chanks[m_length - 1].empty());
    }
}

} // flappy
