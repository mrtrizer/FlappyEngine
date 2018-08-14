#pragma once

#include <array>

#include "IManager.hpp"
#include "ObjectPool.hpp"

class Entity;

class Hierarchy : public EnableSelfHandle<Hierarchy> {
public:
    Hierarchy()
        : m_rootEntity(m_entityPool.create<Entity>())
    {}

    void update(float dt) {
        for (const auto& manager : m_managers)
            manager->update(dt);
        updateEntity(m_rootEntity, dt);
    }

    template <typename ManagerT, typename DerivedServiceT = ManagerT>
    Handle<ManagerT> initManager() {
        auto iter = std::find_if(m_managers.begin(), m_managers.end(), [](const auto& handle) {
            handle.id() == getTypeId<ManagerT>();
        });
        if (iter != m_managers.end())
            *iter = create<DerivedServiceT>(selfHandle());
        else
            m_managers.emplace_back(create<DerivedServiceT>(selfHandle()));
    }

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        size_t size = sizeof(DataT);
        for (const auto& objectPool : m_objectPools) {
            if (objectPool.maxObjectSize() >= size)
                return objectPool.create<DataT>(std::forward<Args>(args)...);
        }
        throw std::runtime_error(sstr("Can't find appropriate object pool for object of size ", size));
    }

private:
    std::vector<StrongHandle<IManager>> m_managers;
    StrongHandle<Entity> m_rootEntity;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 100), ObjectPool(256, 50), ObjectPool(1024, 20) };

    void updateEntity(const StrongHandle<Entity> &entity, float dt);
};
