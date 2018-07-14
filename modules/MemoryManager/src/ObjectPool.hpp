#pragma once

#include "Chank.hpp"

class ObjectPool {
    FORDEBUG(friend class ObjectPoolDebugger);
public:
    ObjectPool(size_t maxObjectSize, size_t capacity)
        : m_maxObjectSize(maxObjectSize)
        , m_bytes(capacity * maxObjectSize)
    {
        USER_ASSERT(capacity > 0);
        USER_ASSERT(maxObjectSize >= sizeof(int));

        m_chanks.reserve(capacity);
        for (size_t i = 0; i < capacity; ++i)
            m_chanks.emplace_back(Chank(&m_bytes[maxObjectSize * i], maxObjectSize));

        m_end = &m_chanks[0];
    }
    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator= (const ObjectPool&) = delete;
    ObjectPool(ObjectPool&&) = delete;
    ObjectPool& operator= (ObjectPool&&) = default;
    ~ObjectPool() {
        // To be sure that m_chanks cleared before m_bytes destroyed
        for (auto& chank : m_chanks) {
            chank.clear();
        }
    }

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


    void onDestroyed (Chank* chank) noexcept {
        // if the chank is not the last element, replace it with the last element
        auto last = m_end - 1;
        if (chank != last)
            chank->moveFrom(last);
        m_end = last;
        m_length--;
    }
};
