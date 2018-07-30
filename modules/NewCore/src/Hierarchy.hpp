#pragma once

#include "Entity.hpp"
#include "IManager.hpp"
#include "ObjectPool.hpp"

class Hierarchy : public EnableSelfHandle<Hierarchy> {
public:
    Hierarchy() {
        m_rootEntity = m_entityPool.create<Entity>();
    }

    void update(float dt) {
        for (auto service : m_services) {
             service->update(dt);
        }
        updateEntity(m_rootEntity, dt);
    }

    template <typename ManagerT, typename DerivedServiceT = ManagerT>
    Handle<ManagerT> initManager() {
        auto iter = m_services.find_if([](const auto& handle) {
           handle.id() == getTypeId<ManagerT>();
        });
        if (iter != m_services.end())
            *iter = create<DerivedServiceT>(selfHandle());
        else
            m_services.emplace_back(m_managerPool);
    }

private:
    std::vector<StrongHandle<IManager>> m_managers;
    StrongHandle<Entity> m_rootEntity;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 100), ObjectPool(256, 50), ObjectPool(1024, 20) };

    template <typename DataT, typename...Args>
    [[nodiscard]] StrongHandle<DataT> create(Args ... args) {
        size_t size = sizeof(DataT);
        for (auto objectPool : m_objectPools) {
            if (objectPool.maxObjectSize() >= size)
                return objectPool.create<DataT>(std::forward<Args>(args)...);
        }
        throw std::runtime_error(sstr("Can't find appropriate object pool for object of size ", size));
    }

    void updateEntity(Handle<Entity> entity, float dt) {
        for (auto updateFunction : entity->updateFunctions()) {
            updateFunction(dt);
        }
        for (auto subEntity : entity->children()) {
            updateEntity(subEntity, dt);
        }
    }
}
