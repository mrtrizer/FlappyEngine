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
    // if the chank is not the last element, move the last element in place of it
    auto last = &m_chanks[m_length - 1];
    if (chank != last)
        chank->moveFrom(last);
    else
        --m_length;
}
