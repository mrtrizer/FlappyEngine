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

Chank::~Chank() {
    if (m_strongHandle != nullptr)
        m_strongHandle->reset();
}

/// Method destroys underlaying instance if it is was initialized.
void Chank::clear() noexcept {
    if (!constructed())
        return;
    DEBUG_ASSERT(m_strongHandle != nullptr);
    DEBUG_ASSERT(m_dataDescructor != nullptr);

    clearHandles();

    m_dataDescructor(m_data);

    m_strongHandle = nullptr;
    m_dataDescructor = nullptr;

    // onDestroyed() should be called in the end
    m_objectPool->onDestroyed(this);
}

bool Chank::constructed() const noexcept {
    return m_dataDescructor != nullptr;
}

void Chank::registerHandle(AnyHandle* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);
    m_handles.emplace_back(handle);

    if (m_hasRemovedHandles && m_handles.size() > 20)
        m_handles.remove_if([](auto itemPtr) { return itemPtr == nullptr; });
}

void Chank::unregisterHandle(void* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);
    DEBUG_ASSERT(!m_handles.empty());

    auto iter = std::find_if(m_handles.begin(), m_handles.end(), [handle](auto itemPtr) {
        return itemPtr == handle;
    });
    DEBUG_ASSERT(iter != m_handles.end());
    *iter = nullptr;

    m_hasRemovedHandles = true;
}

void Chank::clearHandles() noexcept {
    for (auto handle : m_handles)
        if (handle != nullptr)
            handle->invalidate(); // invalidation turns handle into nullptr
    m_handles.clear();
}

} // flappy
