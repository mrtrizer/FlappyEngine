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

    m_end = &m_chanks[0];
}

ObjectPool::~ObjectPool() {
    // To be sure that m_chanks cleared before m_bytes destroyed
    for (auto& chank : m_chanks) {
        chank.clear();
    }
}

void ObjectPool::onDestroyed (Chank* chank) noexcept {
    // if the chank is not the last element, replace it with the last element
    auto last = m_end - 1;
    if (chank != last)
        chank->moveFrom(last);
    m_end = last;
    m_length--;
}
