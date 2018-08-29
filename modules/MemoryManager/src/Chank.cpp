#include "Chank.hpp"

#include "ObjectPool.hpp"

namespace flappy {

Chank::Chank(ObjectPool* objectPool, std::byte* data, size_t size)
    : m_data(data)
    , m_size(size)
    , m_objectPool(objectPool)
{
    DEBUG_ASSERT(m_objectPool != nullptr);
    DEBUG_ASSERT(m_data != nullptr);
    DEBUG_ASSERT(size > 0);
}

Chank::~Chank() noexcept {
    if (m_dataDescructor != nullptr) {
        m_dataDescructor(m_data);
        m_objectPool->onDestroyed(this);
    }
    for (auto handle : m_handles)
        handle->invalidate();
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

    for (auto handle : m_handles)
        handle->invalidate();
    m_handles.clear();

    // onDestroyed() should be called in the end
    m_objectPool->onDestroyed(this);
}

bool Chank::constructed() const noexcept {
    return m_dataDescructor != nullptr;
}

void Chank::registerHandle(AnyHandle* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);

   m_handles.emplace_back(handle);
}

void Chank::unregisterHandle(void* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);
    DEBUG_ASSERT(!m_handles.empty());

    m_handles.remove_if([handle](const auto& item) {
        return item == handle;
    });
}

} // flappy
