#include "Chank.hpp"

#include "ObjectPool.hpp"

Chank::Chank(std::byte* data, size_t size)
    : m_data(data)
    , m_size(size)
{}

Chank::Chank(Chank&& other) noexcept {
    if (other.m_chankFunctions != nullptr) {
        other.m_chankFunctions->updateHandle(other.m_strongHandle, this, other.m_data);
    }
    m_data = other.m_data;
    m_size = other.m_size;
    m_strongHandle = other.m_strongHandle;
    m_chankFunctions = other.m_chankFunctions;
    m_objectPool = other.m_objectPool;
}

Chank::~Chank() {
    if (m_chankFunctions != nullptr) {
        m_chankFunctions->destroy(m_data);
        m_objectPool->onDestroyed(this);
    }
}

void Chank::moveFrom(Chank* chank) {
    DEBUG_ASSERT(chank->m_size == m_size);
    DEBUG_ASSERT(chank->m_data != m_data);

    chank->m_chankFunctions->move(chank->m_data, m_data);
    chank->m_chankFunctions->updateHandle(chank->m_strongHandle, this, m_data);

    m_strongHandle = chank->m_strongHandle;
    m_chankFunctions = chank->m_chankFunctions;
    m_objectPool = chank->m_objectPool;

    chank->clear();
}

/// Method destroys underlaying instance if it is was initialized.
void Chank::clear() noexcept {
    if (constructed()) {
        DEBUG_ASSERT(m_strongHandle != nullptr);
        DEBUG_ASSERT(m_objectPool != nullptr);

        m_chankFunctions->destroy(m_data);
        // FIXME: Looks like bad design. Handle should remove chank via ObjectPool.
        // Destroy callback should be called at the end because it can change class
        auto objectPool = m_objectPool;

        m_strongHandle = nullptr;
        m_chankFunctions = nullptr;
        m_objectPool = nullptr;

        objectPool->onDestroyed(this);
    }
}

bool Chank::constructed() const noexcept {
    return m_chankFunctions != nullptr;
}
