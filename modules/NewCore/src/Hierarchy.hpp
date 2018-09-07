#pragma once

#include <array>

#include "IManager.hpp"
#include "ObjectPool.hpp"
#include "Entity.hpp"

namespace flappy {

class Entity;

class Hierarchy : public EnableSelfHandle<Hierarchy> {
public:
    Hierarchy() {}

    ~Hierarchy() {}

    void update(float dt) {
        for (const auto& manager : m_managers)
            manager.second->update(dt);
        updateEntity(rootEntity(), dt);
    }

    template <typename ManagerT, typename DerivedServiceT = ManagerT>
    Handle<ManagerT> initManager() {
        auto iter = m_managers.find(getTypeId<ManagerT>());
        // Be aware of tricky implicit typecasts
        auto manager = create<DerivedServiceT>(selfHandle());
        auto managerHandle = manager.handle();
        if (iter != m_managers.end())
            iter->second = std::move(manager);
        else
            m_managers.emplace(getTypeId<ManagerT>(), std::move(manager));
        return managerHandle;
    }

    template <typename ManagerT>
    Handle<ManagerT> manager() {
        auto iter = m_managers.find(getTypeId<ManagerT>());
        return iter != m_managers.end() ? iter->second.template handle<ManagerT>() : Handle<ManagerT>();
    }

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        size_t size = sizeof(DataT);
        for (auto& objectPool : m_objectPools) {
            if (objectPool.maxObjectSize() >= size)
                return objectPool.create<DataT>(std::forward<Args>(args)...);
        }
        throw std::runtime_error(sstr("Can't find appropriate object pool for object of size ", size));
    }

    Handle<Entity> rootEntity() {
        if (!m_rootEntity)
            m_rootEntity = m_entityPool.create<Entity>(selfHandle());
        return m_rootEntity;
    }

private:
    std::unordered_map<TypeId, StrongHandle<IManager>> m_managers;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    StrongHandle<Entity> m_rootEntity;
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 100), ObjectPool(256, 50), ObjectPool(1024, 20) };

    void updateEntity(const Handle<Entity> &entity, float dt);
};

} // flappy
