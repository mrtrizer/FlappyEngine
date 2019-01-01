#include "AnyHandle.hpp"

#include "Chank.hpp"
#include "AnyStrongHandle.hpp"

namespace flappy {

template <typename T>
class Handle;

AnyHandle::AnyHandle(Chank* chank) noexcept {
    DEBUG_ASSERT(chank != nullptr);
    chank->registerHandle(this);
    m_chank = chank;
}

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

bool AnyHandle::isSame(const AnyStrongHandle& other) const {
    return m_chank == other.m_chank;
}

TypeId AnyHandle::typeId() const noexcept {
    if (m_chank == nullptr)
        return {};
    return m_chank->typeId();
}

ObjectId AnyHandle::objectId() const noexcept {
    return m_chank->objectId();
}

void AnyHandle::setNewChank(Chank* chank) noexcept {
    if (m_chank != nullptr)
        m_chank->unregisterHandle(this);
    m_chank = chank;
    if (chank != nullptr)
        chank->registerHandle(this);
}

void AnyHandle::invalidate() noexcept {
    setNewChank(nullptr);
}

bool operator==(const AnyStrongHandle& a, const AnyHandle& b) { return a.m_chank == b.m_chank; }

bool operator==(const AnyHandle& a, const AnyStrongHandle& b) { return a.m_chank == b.m_chank; }

} // flappy
