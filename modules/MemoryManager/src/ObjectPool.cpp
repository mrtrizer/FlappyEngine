#include "ObjectPool.hpp"

ObjectPool::ObjectPool(size_t maxObjectSize, size_t capacity)
    : m_maxObjectSize(maxObjectSize)
    , m_bytes(capacity * maxObjectSize)
{
    USER_ASSERT(capacity > 0);
    USER_ASSERT(maxObjectSize >= sizeof(int));

    m_chanks.reserve(capacity);
    for (size_t i = 0; i < capacity; ++i)
        m_chanks.emplace_back(Chank(&m_bytes[maxObjectSize * i], maxObjectSize));
}

ObjectPool::~ObjectPool() {
    // To be sure that m_chanks cleared before m_bytes destroyed
    for (auto& chank : m_chanks) {
        chank.clear();
    }
}

void ObjectPool::onDestroyed (Chank* chank) noexcept {
    DEBUG_ASSERT(m_length > 0);
    if (chank == &m_chanks[m_length - 1]) {
        do
            --m_length;
        while (m_length > 0 && m_chanks[m_length - 1].empty());
    }
}

Chank* ObjectPool::findEmptyChank() noexcept {
    if (m_length != m_chanks.size())
        return &m_chanks[m_length];
    for (auto& chank : m_chanks)
        if (chank.empty())
            return &chank;
    return nullptr;
}
