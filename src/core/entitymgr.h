#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include "entity.h"
#include "flappyapp.h"
#include "manager.h"

namespace flappy {

class Entity;

class EntityMgr: public Manager<EntityMgr> {
public:    
    void update(TimeDelta dt);

    void remove(std::shared_ptr<Entity> entity);

    void reset();

    std::shared_ptr<Entity> create(std::function<void(std::shared_ptr<Entity>)> func = [](std::shared_ptr<Entity>){});

    std::shared_ptr<Entity> find(std::function<bool(const Entity*)> check);

    std::list<std::shared_ptr<Entity>> findAll(std::function<bool(const Entity*)> check);

    template <typename ... Components>
    void each(std::function<void(std::shared_ptr<Entity>)> func) {
        for (auto entity: m_entities) {
            if (check<Components...>(entity))
                func(entity);
        }
    }
private:
    std::list<std::shared_ptr<Entity>> m_entities;
    std::list<std::shared_ptr<Entity>> m_removeList;
    
    template <typename ComponentT = void, typename ... Components>
    bool check(std::shared_ptr<Entity> entity) {
        return check<Components...>(entity) && (entity->get<ComponentT>() != nullptr);
    }
};

template <> inline bool EntityMgr::check <void> (std::shared_ptr<Entity>) {
    return true;
}

using EP = std::shared_ptr<Entity>;
#define EM MGR<EntityMgr>()

} // flappy
