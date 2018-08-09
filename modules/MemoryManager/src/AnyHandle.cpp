#include "AnyHandle.hpp"

#include "AnyStrongHandle.hpp"

template <typename T>
class Handle;

AnyHandle::AnyHandle(AnyStrongHandle& strongHandle) noexcept
    : m_strongHandle(&strongHandle)
{
    m_strongHandle->registerHandle(this);
}

AnyHandle& AnyHandle::operator=(AnyStrongHandle& strongHandle) noexcept {
    setNewHandle(&strongHandle);
    return *this;
}

// Destructor should not be virtual in this case
AnyHandle::~AnyHandle() {
    if (m_strongHandle != nullptr)
        m_strongHandle->unregisterHandle(this);
}

bool AnyHandle::isValid() noexcept {
    return m_strongHandle != nullptr && m_strongHandle->isValid();
}

TypeId AnyHandle::typeId() const noexcept {
    return m_strongHandle->typeId();
}

void AnyHandle::setNewHandle(AnyStrongHandle* strongHandle) noexcept {
    if (m_strongHandle != nullptr)
        m_strongHandle->unregisterHandle(this);
    m_strongHandle = reinterpret_cast<AnyStrongHandle*>(strongHandle);
    if (strongHandle != nullptr)
        strongHandle->registerHandle(this);
}

void AnyHandle::invalidate() noexcept {
    DEBUG_ASSERT(m_strongHandle != nullptr);

    m_strongHandle = nullptr;
}

// strongHandlePtr is void* to support anonymous handles
void AnyHandle::updateStrongHandle(void* strongHandlePtr) noexcept {
    DEBUG_ASSERT(m_strongHandle != nullptr);
    DEBUG_ASSERT(strongHandlePtr != nullptr);

    m_strongHandle = static_cast<AnyStrongHandle*>(strongHandlePtr);
}

void AnyHandle::registerInStrongHandle() noexcept {
    m_strongHandle->registerHandle(this);
}
