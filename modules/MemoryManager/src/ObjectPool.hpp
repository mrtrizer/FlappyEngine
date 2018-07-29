#pragma once

#include "Chank.hpp"

template <typename ObjectT>
class EnableSelfHandle {
    friend class ObjectPool; // to assign self handle
protected:
    Handle<ObjectT> selfHandle() { return m_selfHandle; }
private:
    Handle<ObjectT> m_selfHandle;
};

// TODO: Moving objects between pools

class ObjectPool {
    FORDEBUG(friend class ObjectPoolDebugger);
public:
    ObjectPool(size_t maxObjectSize, size_t capacity);
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator= (const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator= (ObjectPool&&) = default;
    ~ObjectPool();

    // TODO: Autoresize on exeeding the limit of chanks (should be easy with vectors)

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        DEBUG_ASSERT(m_end != nullptr);

        static_assert(std::is_class<DataT>(), "ObjectPool doesn't support basic types.");
        USER_ASSERT_MSG(sizeof(DataT) <= m_maxObjectSize, "DataT exeeds max size (", sizeof(DataT), " > ",  m_maxObjectSize, ")");
        USER_ASSERT_MSG(m_length < m_chanks.size(), "You have reached limit of chanks. Max: ", m_chanks.size());

        auto end = m_end;
        auto length = m_length;

        try {
            auto strongHandle = end->template construct<DataT>(
                        std::bind(&ObjectPool::onDestroyed, this, std::placeholders::_1),
                        std::forward<Args>(args)...);
            setSelfHandle(&strongHandle);
            m_end = end + 1;
            m_length = length + 1;
            return strongHandle;
        } catch (...) {
            m_end = end;
            m_length = length;
            throw;
        }
    }

private:
    size_t m_maxObjectSize = 0;
    std::vector<Chank> m_chanks;
    std::vector<std::byte> m_bytes;
    Chank* m_end = nullptr;
    int m_length = 0;

    void onDestroyed (Chank* chank) noexcept;

    template <typename ObjectT>
    void setSelfHandle(StrongHandle<ObjectT> *object) {
        if constexpr (std::is_base_of<ObjectT, EnableSelfHandle<ObjectT>>()) {
            (*object)->m_selfHandle = object->handle();
        }
    }

//    template <typename ObjectT, typename = std::enable_if_t<std::is_base_of<ObjectT, EnableSelfHandle<ObjectT>>::value>>
//    inline void setSelfHandle(StrongHandle<ObjectT>* object) {
//        (*object)->m_selfHandle = object->handle();
//    }

//    template <typename ObjectT, typename = std::enable_if_t<!std::is_base_of<ObjectT, EnableSelfHandle<ObjectT>>::value>>
//    inline void setSelfHandle(StrongHandle<ObjectT>* object) {
//        // do nothing if not a base of EnableSelfHandle
//    }
};
