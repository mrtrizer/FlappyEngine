#include "AnyHandle.hpp"

#include "Chank.hpp"
#include "AnyStrongHandle.hpp"

namespace flappy {

template <typename T>
class Handle;

AnyHandle::AnyHandle(const AnyStrongHandle& strongHandle) noexcept
    : m_chank(strongHandle.m_chank)
{
    if (m_chank != nullptr)
        m_chank->registerHandle(this);
}

AnyHandle& AnyHandle::operator=(const AnyStrongHandle& strongHandle) noexcept {
    setNewChank(strongHandle.m_chank);
    return *this;
}

AnyHandle::~AnyHandle() {
    if (m_chank != nullptr)
        m_chank->unregisterHandle(this);
}

bool AnyHandle::isValid() const noexcept {
    return m_chank != nullptr;
}

TypeId AnyHandle::typeId() const noexcept {
    if (m_chank == nullptr)
        return {};
    return m_chank->typeId();
}

void AnyHandle::setNewChank(Chank* chank) noexcept {
    if (m_chank != nullptr)
        m_chank->unregisterHandle(this);
    m_chank = chank;
    if (chank != nullptr)
        chank->registerHandle(this);
}

void AnyHandle::invalidate() noexcept {
    DEBUG_ASSERT(m_chank != nullptr);

    setNewChank(nullptr);
}

} // flappy
