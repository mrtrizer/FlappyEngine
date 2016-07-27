#ifndef EntityManager_h
#define EntityManager_h

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include <core/entity.h>

#include "manager.h"

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

using EP = std::shared_ptr<Entity>;

namespace EM {
    void create(std::function<void(std::shared_ptr<Entity>)> func);
    void remove(std::shared_ptr<Entity> entity);
    std::list<std::shared_ptr<Entity>> findall(std::function<bool(const Entity*)> check);
    std::shared_ptr<Entity> find(std::function<bool(const Entity*)> check);
    template <typename ... Components>
    void each(std::function<void(std::shared_ptr<Entity>)> func) {
        EntityMgr::getInst()->each<Components...>(func);
    }
}

#endif /* EntityManager_h */
