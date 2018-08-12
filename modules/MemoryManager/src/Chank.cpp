#include "Chank.hpp"

#include "ObjectPool.hpp"

Chank::Chank(ObjectPool* objectPool, std::byte* data, size_t size)
    : m_data(data)
    , m_size(size)
    , m_objectPool(objectPool)
{
    DEBUG_ASSERT(m_objectPool != nullptr);
    DEBUG_ASSERT(m_data != nullptr);
    DEBUG_ASSERT(size > 0);
}

Chank::~Chank() {
    if (m_dataDescructor != nullptr) {
        m_dataDescructor(m_data);
        m_objectPool->onDestroyed(this);
    }
}

/// Method destroys underlaying instance if it is was initialized.
void Chank::clear() noexcept {
    if (!constructed())
        return;
    DEBUG_ASSERT(m_strongHandle != nullptr);
    DEBUG_ASSERT(m_dataDescructor != nullptr);

    m_dataDescructor(m_data);

    m_strongHandle = nullptr;
    m_dataDescructor = nullptr;

    // onDestroyed() should be called in the end
    m_objectPool->onDestroyed(this);
}

bool Chank::constructed() const noexcept {
    return m_dataDescructor != nullptr;
}
