#pragma once

#include <vector>

#include "Chank.hpp"
#include "Handle.hpp"

template <typename ObjectT>
class EnableSelfHandle {
    friend class ObjectPool; // to assign self handle
protected:
    Handle<ObjectT> selfHandle() { return m_selfHandle; }
private:
    Handle<ObjectT> m_selfHandle;
};

class ObjectPool {
    FORDEBUG(friend class ObjectPoolDebugger);
public:
    ObjectPool(size_t maxObjectSize, size_t capacity);
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator= (const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator= (ObjectPool&&) = default;
    ~ObjectPool();

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        static_assert(std::is_class<DataT>(), "ObjectPool doesn't support basic types.");
        USER_ASSERT_MSG(sizeof(DataT) <= m_maxObjectSize, "DataT exeeds max size (", sizeof(DataT), " > ",  m_maxObjectSize, ")");
        DEBUG_ASSERT(m_length <= m_chanks.size());

        auto emptyChank = findEmptyChank();
        DEBUG_ASSERT(emptyChank == nullptr || emptyChank->empty());
        if (emptyChank == nullptr)
            throw FlappyException("No empty memory chanks left. Review parameters of the object pool!");
        try {
            auto strongHandle = emptyChank->construct<DataT>(
                        std::bind(&ObjectPool::onDestroyed, this, std::placeholders::_1),
                        std::forward<Args>(args)...);
            if constexpr (std::is_base_of<EnableSelfHandle<DataT>, DataT>::value)
                strongHandle->m_selfHandle = strongHandle.handle();
            if (&m_chanks[m_length] == emptyChank)
                ++m_length;
            return strongHandle;
        } catch (...) {
            throw;
        }
    }

    size_t maxObjectSize() const {
        return m_maxObjectSize;
    }

private:
    size_t m_maxObjectSize = 0;
    std::vector<Chank> m_chanks;
    std::vector<std::byte> m_bytes;
    size_t m_length = 0;

    void onDestroyed (Chank* chank) noexcept;

    Chank* findEmptyChank() noexcept;
};
