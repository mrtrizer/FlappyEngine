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
    for (auto handle : m_handles)
        handle->invalidate();
    if (m_chank != nullptr)
        m_chank->clear();
}

bool AnyStrongHandle::isValid() const noexcept {
    return m_dataPointer != nullptr;
}

TypeId AnyStrongHandle::typeId() const noexcept{
    return m_typeId;
}

AnyStrongHandle::AnyStrongHandle(TypeId typeId,
                void* dataPointer,
                Chank* chank) noexcept
    : m_typeId(typeId)
    , m_dataPointer(dataPointer)
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
    for (auto handle : m_handles)
        handle->invalidate();
    m_handles.clear();
    if (m_chank != nullptr)
        m_chank->clear();
    m_chank = nullptr;
    m_dataPointer = nullptr;
}

void AnyStrongHandle::registerHandle(AnyHandle* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);

   m_handles.emplace_back(handle);
}

void AnyStrongHandle::unregisterHandle(void* handle) noexcept {
    DEBUG_ASSERT(handle != nullptr);
    DEBUG_ASSERT(!m_handles.empty());

    m_handles.remove_if([handle](const auto& item) {
        return item == handle;
    });
}

void AnyStrongHandle::moveFromStrongHandle(AnyStrongHandle&& strongHandle) {
    m_typeId = strongHandle.m_typeId;
    m_dataPointer = strongHandle.m_dataPointer;
    strongHandle.m_dataPointer = nullptr;
    auto chank = m_chank = strongHandle.m_chank;
    strongHandle.m_chank = nullptr;
    std::move(strongHandle.m_handles.begin(), strongHandle.m_handles.end(), std::back_inserter(m_handles));
    strongHandle.m_handles.clear();
    for (auto handle : m_handles)
        handle->updateStrongHandle(this);
    if (chank != nullptr)
        chank->m_strongHandle = this;
}
