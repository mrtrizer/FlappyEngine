#pragma once

#include "ObjectPool.hpp"

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
        return size_t(strongHandle.m_chank->m_data - &m_objectPool.m_bytes.front()) / m_objectPool.m_maxObjectSize;
    }

    void printState() {
        std::stringstream ss;
        for (const auto& chank : m_objectPool.m_chanks) {
            ss << (chank.constructed() ? "1" : "0");
        }
        std::cout << ss.str() << std::endl;
    }

private:
    ObjectPool& m_objectPool;
};
#endif
