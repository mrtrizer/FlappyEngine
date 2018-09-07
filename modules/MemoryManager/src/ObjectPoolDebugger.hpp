#pragma once

#include "ObjectPool.hpp"

namespace flappy {

#ifndef NDEBUG
class ObjectPoolDebugger {
public:
    explicit ObjectPoolDebugger(ObjectPool& objectPool)
        : m_objectPool(objectPool)
    {}

    template<typename StrongHandleT>
    int getChankIndex(StrongHandleT& strongHandle) {
        if (!strongHandle.isValid())
            return -1;
        return size_t(strongHandle.m_chank->m_data - m_objectPool.m_bytes.data()) / m_objectPool.m_maxObjectSize;
        return 0;
    }

    void printState() {
        std::stringstream ss;
        for (size_t i = 0; i < m_objectPool.m_capacity; ++i) {
            ss << (m_objectPool.m_chanks[i].constructed() ? "1" : "0");
        }
        std::cout << ss.str() << std::endl;
    }

private:
    ObjectPool& m_objectPool;
};
#endif

} // flappy
