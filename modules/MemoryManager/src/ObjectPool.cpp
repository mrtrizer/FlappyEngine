#include "ObjectPool.hpp"

ObjectPool::ObjectPool(size_t maxObjectSize, size_t capacity)
    : m_maxObjectSize(maxObjectSize)
    , m_bytes()
    , m_capacity(capacity)
{
    USER_ASSERT(capacity > 0);
    USER_ASSERT(maxObjectSize >= sizeof(int));

    // TODO: Rework with using std::allocator or std::vector or at least operator new
    m_bytes = (std::byte*)malloc(capacity * maxObjectSize);
    m_chanks = (Chank*)malloc(sizeof(Chank) * capacity);
    for (size_t i = 0; i < capacity; ++i)
        new (m_chanks + i) Chank(this, &m_bytes[maxObjectSize * i], maxObjectSize);
}

ObjectPool::~ObjectPool() {
    // To be sure that m_chanks cleared before m_bytes destroyed
    for (size_t i = 0; i < m_capacity; ++i) {
        m_chanks[i].clear();
    }
    free(m_chanks);
    free(m_bytes);
}

void ObjectPool::onDestroyed (Chank* chank) noexcept {
    DEBUG_ASSERT(m_length > 0);
    if (chank == &m_chanks[m_length - 1]) {
        do
            --m_length;
        while (m_chanks[m_length - 1].empty() && m_length > 0);
    }
}


