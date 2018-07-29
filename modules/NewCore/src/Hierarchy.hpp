#pragma once

#include "Entity.hpp"
#include "IService.hpp"
#include "ObjectPool.hpp"

class Hierarchy {
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

    template <typename ServiceT, typename DerivedServiceT>
    void setService(StrongHandle<DerivedServiceT>&& service) {
        auto iter = m_services.find_if([](const auto& handle) {
           handle.id() == getTypeId<ServiceT>();
        });
        if (iter != m_services.end())
            *iter = service;
        else
            m_services.emplace_back(std::move(service));
    }

private:
    std::vector<StrongHandle<IService>> m_services;
    StrongHandle<Entity> m_rootEntity;
    ObjectPool m_entityPool { sizeof(Entity), 1000 };
    std::array<ObjectPool, 3> m_objectPools { ObjectPool(64, 100), ObjectPool(256, 50), ObjectPool(1024, 20) };

    void updateEntity(Handle<Entity> entity, float dt) {
        for (auto updateFunction : entity->updateFunctions()) {
            updateFunction(dt);
        }
        for (auto subEntity : entity->children()) {
            updateEntity(subEntity, dt);
        }
    }
}
