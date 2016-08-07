#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include "entity.h"
#include "flappyapp.h"
#include "manager.h"

namespace flappy {

using namespace std;

class Entity;

class EntityMgr: public Manager<EntityMgr> {
public:    
    void update(TimeDelta dt);

    void remove(shared_ptr<Entity> entity);

    void reset();

    shared_ptr<Entity> create(function<void(shared_ptr<Entity>)> func = [](shared_ptr<Entity>){});

    shared_ptr<Entity> find(function<bool(const Entity*)> check);

    list<shared_ptr<Entity>> findAll(function<bool(const Entity*)> check);

    template <typename ... Components>
    void each(function<void(shared_ptr<Entity>)> func) {
        for (auto entity: m_entities) {
            if (check<Components...>(entity))
                func(entity);
        }
    }
private:
    list<shared_ptr<Entity>> m_entities;
    list<shared_ptr<Entity>> m_removeList;
    
    template <typename ComponentT = void, typename ... Components>
    bool check(shared_ptr<Entity> entity) {
        return check<Components...>(entity) && (entity->get<ComponentT>() != nullptr);
    }
};

using EP = shared_ptr<Entity>;

} // flappy
