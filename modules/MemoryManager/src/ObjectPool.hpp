#pragma once

#include <vector>

#include "Chank.hpp"
#include "Handle.hpp"
#include "EnableSelfHandle.hpp"

namespace flappy {

class ObjectPool {
    FORDEBUG(friend class ObjectPoolDebugger);
    friend class Chank; // for calling of onDestroyed()
public:
    ObjectPool(size_t maxObjectSize, size_t capacity);
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator= (const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator= (ObjectPool&&) = delete;
    ~ObjectPool() noexcept;

    template <typename DataT, typename...ArgT>
    [[nodiscard]] StrongHandle<DataT> create(ArgT&& ... args) {
        return createWithId<DataT>(0, std::forward<ArgT>(args)...);
    }

    template <typename DataT, typename...ArgT>
    [[nodiscard]] StrongHandle<DataT> createWithId(ObjectId objectId, ArgT&& ... args) {
        static_assert(std::is_class<DataT>(), "ObjectPool doesn't support basic types.");
        USER_ASSERT_MSG(sizeof(DataT) <= m_maxObjectSize, "DataT exeeds max size (", sizeof(DataT), " > ",  m_maxObjectSize, ")");
        DEBUG_ASSERT(m_length <= m_capacity);

        auto emptyChank = findEmptyChank();
        DEBUG_ASSERT(emptyChank == nullptr || emptyChank->empty());
        if (emptyChank == nullptr)
            throw FlappyException("No empty memory chanks left. Review parameters of the object pool!");
        try {
            if constexpr (std::is_base_of<__EnableSelfHandleMarker, DataT>::value)
                emptyChank->data<DataT>()->m_selfChankPtr = emptyChank;
            
            // We should increment length before construction because
            // a new object can be constructed during the current cunstruction.
            // I also don't decrement m_length in catch block because
            // a new object can already be constructed after the current one.
            if (&m_chanks[m_length] == emptyChank)
                ++m_length;
            auto strongHandle = emptyChank->construct<DataT>(std::forward<ArgT>(args)...);
            emptyChank->m_objectId = objectId;
            return strongHandle;
        } catch (...) {
            throw;
        }
    }

    size_t maxObjectSize() const {
        return m_maxObjectSize;
    }

private:
    using ChankAllocatorT = std::allocator<Chank>;
    using ByteAllocatorT = std::allocator<std::byte>;

    size_t m_maxObjectSize = 0;
    Chank* m_chanks; // array
    std::vector<std::byte> m_bytes;
    size_t m_length = 0;
    size_t m_capacity = 0;
    ChankAllocatorT m_chankAllocator;
    ByteAllocatorT m_byteAllocator;

    void onDestroyed (Chank* chank) noexcept;

    inline Chank* findEmptyChank() noexcept {
        if (m_length != m_capacity)
            return &m_chanks[m_length];
        for (size_t i = 0; i < m_capacity; ++i) {
            if (m_chanks[i].empty())
                return &m_chanks[i];
        }
        return nullptr;
    }
};

} // flappy
