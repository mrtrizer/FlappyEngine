#include "AnyStrongHandle.hpp"

#include "Chank.hpp"
#include "AnyHandle.hpp"

namespace flappy {

AnyStrongHandle& AnyStrongHandle::operator=(std::nullptr_t) noexcept {
    reset();
    return *this;
}

AnyStrongHandle::AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept {
    moveFromStrongHandle(std::move(strongHandle));
}

AnyStrongHandle& AnyStrongHandle::operator=(AnyStrongHandle&& strongHandle) noexcept {
    if (&strongHandle != this)
        moveFromStrongHandle(std::move(strongHandle));
    DEBUG_ASSERT(m_chank == nullptr || this == m_chank->m_strongHandle);
    return *this;
}

AnyStrongHandle::~AnyStrongHandle() noexcept {
    DEBUG_ASSERT(m_chank == nullptr || this == m_chank->m_strongHandle);
    if (m_chank != nullptr)
        m_chank->clear();
}

bool AnyStrongHandle::isValid() const noexcept {
    return m_dataPointer != nullptr;
}

TypeId AnyStrongHandle::typeId() const noexcept {
    return m_chank->typeId();
}

ObjectId AnyStrongHandle::objectId() const noexcept {
    return m_chank->objectId();
}

AnyStrongHandle::AnyStrongHandle(void* dataPointer,
                Chank* chank) noexcept
    : m_dataPointer(dataPointer)
    , m_chank(chank)
{}

void AnyStrongHandle::updatePointer(void* dataPointer, Chank* chank) noexcept {
    DEBUG_ASSERT(dataPointer != nullptr);
    DEBUG_ASSERT(chank != nullptr);
    DEBUG_ASSERT(m_dataPointer != nullptr);
    DEBUG_ASSERT(m_chank != nullptr);

    m_dataPointer = dataPointer;
    m_chank = chank;
}

void AnyStrongHandle::reset() noexcept {
    DEBUG_ASSERT(m_chank == nullptr || this == m_chank->m_strongHandle);
    if (m_chank != nullptr)
        m_chank->clear();
    m_chank = nullptr;
    m_dataPointer = nullptr;
}

void AnyStrongHandle::moveFromStrongHandle(AnyStrongHandle&& strongHandle) noexcept {
    // Reset first. AnyStrongHandle can already hold some data.
    reset();
    m_dataPointer = strongHandle.m_dataPointer;
    strongHandle.m_dataPointer = nullptr;
    auto chank = m_chank = strongHandle.m_chank;
    strongHandle.m_chank = nullptr;
    if (chank != nullptr)
        chank->m_strongHandle = this;
    DEBUG_ASSERT(m_chank == nullptr || this == m_chank->m_strongHandle);
}

} // flappy
