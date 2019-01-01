#pragma once

#include <array>
#include <random>

#include <Utility.hpp>

#include "ObjectPool.hpp"
#include "StrongHandle.hpp"

namespace flappy {

class MemoryManager {
public:
    MemoryManager();

    template <typename DataT, typename...ArgT>
    [[nodiscard]] StrongHandle<DataT> create(ArgT&& ... args) {
        return createWithId<DataT>(genRandomId(), std::forward<ArgT>(args)...);
    }
    
    template <typename DataT, typename...ArgT>
    [[nodiscard]] StrongHandle<DataT> createWithId(ObjectId objectId, ArgT&& ... args) {
        size_t size = sizeof(DataT);
        for (auto& objectPool : m_objectPools) {
            if (objectPool.maxObjectSize() >= size) {
                auto strongHandle = objectPool.createWithId<DataT>(objectId, std::forward<ArgT>(args)...);
                m_objectHandleMap.emplace(objectId, strongHandle);
                return strongHandle;
            }
        }
        throw FlappyException(sstr("Can't find appropriate object pool for object of size ", size));
    }
    
    AnyHandle findObject(ObjectId objectId) {
        auto foundIter = m_objectHandleMap.find(objectId);
        if (foundIter != m_objectHandleMap.end())
            return foundIter->second;
        return nullptr;
    }
    
private:
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 200), ObjectPool(256, 150), ObjectPool(1024, 100) };
    std::unordered_map<ObjectId, AnyHandle> m_objectHandleMap;
    std::random_device m_randomDevice;
    std::mt19937 m_generator;
    std::uniform_int_distribution<ObjectId> m_distribution;
    
    ObjectId genRandomId() {
        return m_distribution(m_generator);
    }
};

} // flappy
