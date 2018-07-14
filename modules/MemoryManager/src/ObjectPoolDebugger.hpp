#pragma once

#include "ObjectPool.hpp"

class ObjectPoolDebugger {
public:
    ObjectPoolDebugger(const ObjectPool& objectPool)
        : m_objectPool(objectPool)
    {}

    int getChankIndex(const StrongHandle& strongHandle) {
        return size_t(strongHandle.chank->m_data - m_objectPool.m_bytes[0]) / m_objectPool.m_maxObjectSize;
    }

private:
    ObjectPool& objectPool;
};
