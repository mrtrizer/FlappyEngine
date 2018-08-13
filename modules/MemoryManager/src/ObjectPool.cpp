#include "ObjectPool.hpp"

#include <memory>

ObjectPool::ObjectPool(size_t maxObjectSize, size_t capacity)
    : m_maxObjectSize(maxObjectSize)
    , m_bytes()
    , m_capacity(capacity)
{
    USER_ASSERT(capacity > 0);
    USER_ASSERT(maxObjectSize >= sizeof(int));

    // TODO: Rework with using std::allocator or std::vector or at least operator new
    m_bytes = std::allocator_traits<ByteAllocatorT>::allocate(m_byteAllocator, capacity * maxObjectSize);
    m_chanks = std::allocator_traits<ChankAllocatorT>::allocate(m_chankAllocator, capacity);
    for (size_t i = 0; i < capacity; ++i)
        std::allocator_traits<ChankAllocatorT>::construct(m_chankAllocator, m_chanks + i, this, &m_bytes[maxObjectSize * i], maxObjectSize);
}

ObjectPool::~ObjectPool() {
    if (m_chanks == nullptr)
        return;
    for (size_t i = 0; i < m_capacity; ++i)
        std::allocator_traits<ChankAllocatorT>::destroy(m_chankAllocator, m_chanks + i);
    std::allocator_traits<ChankAllocatorT>::deallocate(m_chankAllocator, m_chanks, m_capacity);
    std::allocator_traits<ByteAllocatorT>::deallocate(m_byteAllocator, m_bytes, m_capacity * m_maxObjectSize);
}

void ObjectPool::onDestroyed (Chank* chank) noexcept {
    DEBUG_ASSERT(m_length > 0); // 1
    if (chank == &m_chanks[m_length - 1]) { // 1 - 1
        do
            --m_length; // 1 - 1 = 0
        while (m_length > 0 && m_chanks[m_length - 1].empty());
    }
}


