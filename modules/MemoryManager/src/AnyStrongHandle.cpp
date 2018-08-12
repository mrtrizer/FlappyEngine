#include "AnyStrongHandle.hpp"

#include "Chank.hpp"
#include "AnyHandle.hpp"

AnyStrongHandle& AnyStrongHandle::operator=(std::nullptr_t) noexcept {
    reset();
    return *this;
}

AnyStrongHandle::AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept {
    moveFromStrongHandle(std::move(strongHandle));
}

AnyStrongHandle& AnyStrongHandle::operator=(AnyStrongHandle&& strongHandle) noexcept {
    moveFromStrongHandle(std::move(strongHandle));
    return *this;
}

AnyStrongHandle::~AnyStrongHandle() {
    if (m_chank != nullptr)
        m_chank->clear();
}

bool AnyStrongHandle::isValid() const noexcept {
    return m_dataPointer != nullptr;
}

TypeId AnyStrongHandle::typeId() const noexcept{
    return m_chank->typeId();
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
    if (m_chank != nullptr)
        m_chank->clear();
    m_chank = nullptr;
    m_dataPointer = nullptr;
}



void AnyStrongHandle::moveFromStrongHandle(AnyStrongHandle&& strongHandle) {
    m_dataPointer = strongHandle.m_dataPointer;
    strongHandle.m_dataPointer = nullptr;
    auto chank = m_chank = strongHandle.m_chank;
    strongHandle.m_chank = nullptr;
    if (chank != nullptr)
        chank->m_strongHandle = this;
}
