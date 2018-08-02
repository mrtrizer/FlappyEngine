#pragma once

#include <vector>

#include "Utility.hpp"

class Chank;

template <typename DataT>
class StrongHandle;

struct HandleCalls {
    void* rawPointer;
    std::function<void(void)> invalidate;
    std::function<void(void* strongHandle)> updateStrongHandle;
};

class AnyStrongHandle {
    template <typename T>
    friend class Handle; // to register/unregister handles
    friend class AnyHandle; // to register/unregister handles
public:
    AnyStrongHandle(AnyStrongHandle&& strongHandle) noexcept {
        moveFromStrongHandle(std::move(strongHandle));
    }

    ~AnyStrongHandle() {
        reset();
    }

    bool isValid() const noexcept {
        return m_dataPointer != nullptr;
    }

    TypeId typeId() const noexcept{
        return m_typeId;
    }

protected:
    TypeId m_typeId;
    void* m_dataPointer = nullptr;
    Chank* m_chank = nullptr;
    std::vector<HandleCalls> m_handles;

    AnyStrongHandle(TypeId typeId = 0,
                     void* dataPointer = nullptr,
                     Chank* chank = nullptr) noexcept
        : m_typeId(typeId)
        , m_dataPointer(dataPointer)
        , m_chank(chank)
    {}

    void reset() noexcept {
        for (auto handle : m_handles)
            handle.invalidate();
        m_handles.clear();
        if (m_chank != nullptr)
            clearChank (m_chank);
        m_chank = nullptr;
        m_dataPointer = nullptr;
    }

    template <typename T>
    void registerHandle(T* handle) noexcept {
        DEBUG_ASSERT(handle != nullptr);

       m_handles.emplace_back(HandleCalls{
           handle,
           std::bind(&T::invalidate, handle),
           std::bind(&T::updateStrongHandle, handle, std::placeholders::_1)
       });
    }

    void unregisterHandle(void* handle) noexcept {
        DEBUG_ASSERT(handle != nullptr);
        DEBUG_ASSERT(!m_handles.empty());

        if (m_handles.back().rawPointer != handle) {
            auto iter = std::find_if(m_handles.begin(), m_handles.end(), [handle](const auto& item) {
                return item.rawPointer == handle;
            });
            DEBUG_ASSERT(iter != m_handles.end());
            *iter = std::move(m_handles.back());
        }

        m_handles.erase(std::prev(m_handles.end()), m_handles.end());
    }

    void moveFromStrongHandle(AnyStrongHandle&& strongHandle) {
        m_typeId = strongHandle.m_typeId;
        m_dataPointer = strongHandle.m_dataPointer;
        strongHandle.m_dataPointer = nullptr;
        auto chank = m_chank = strongHandle.m_chank;
        strongHandle.m_chank = nullptr;
        std::move(strongHandle.m_handles.begin(), strongHandle.m_handles.end(), std::back_inserter(m_handles));
        strongHandle.m_handles.clear();
        for (auto handle : m_handles)
            handle.updateStrongHandle(this);
        if (chank != nullptr)
            updateChankStrongHandle (chank, this);
    }

    void updatePointer(void* dataPointer, Chank* chank) noexcept {
        DEBUG_ASSERT(dataPointer != nullptr);
        DEBUG_ASSERT(chank != nullptr);
        DEBUG_ASSERT(m_dataPointer != nullptr);
        DEBUG_ASSERT(m_chank != nullptr);

        m_dataPointer = dataPointer;
        m_chank = chank;
    }

private:
    template <typename ChankT>
    void clearChank(ChankT* chank) {
        chank->clear();
    }

    template <typename ChankT>
    void updateChankStrongHandle(ChankT* chank, AnyStrongHandle* strongHandle) {
        chank->m_strongHandle = strongHandle;
    }
};
