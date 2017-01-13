#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include <ManagerList.h>
#include <Manager.h>

#include "Entity.h"

namespace flappy {

class EntityManager: public Manager {
public:    
    void update(TimeDelta dt);

    /// Entity will be removed right on update
    void remove(std::shared_ptr<Entity> entity);

    /// All entities will be removed on next update
    void reset();

    std::shared_ptr<Entity> create(std::function<void(const std::shared_ptr<Entity>&)> func);

    std::shared_ptr<Entity> create();

    std::shared_ptr<Entity> add(std::shared_ptr<Entity>);

    std::shared_ptr<Entity> find(std::function<bool(const std::shared_ptr<Entity>&)> check);

    std::list<std::shared_ptr<Entity>> findAll(std::function<bool(const std::shared_ptr<Entity>&)> check);

    std::list<std::shared_ptr<Entity>> entities() const {return m_entities;}

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
        return check<Components...>(entity) && (entity->findComponent<ComponentT>());
    }
};

template <> inline bool EntityManager::check <void> (std::shared_ptr<Entity>) {
    return true;
}

using EP = const std::shared_ptr<Entity>&;

} // flappy
