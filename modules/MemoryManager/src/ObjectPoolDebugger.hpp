#pragma once

#include "ObjectPool.hpp"

template <size_t ChankSize>
class ObjectPoolDebugger {
public:
    ObjectPoolDebugger(const ObjectPool<ChankSize>& objectPool)
        : m_objectPool(objectPool)
    {}

    int getChankIndex(const StrongHandle& strongHandle) {

    }

private:
    ObjectPool<ChankSize>& objectPool;
};
