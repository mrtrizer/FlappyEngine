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
    void update(TimeDelta dt) {
        for (auto entity: m_removeList)
            m_entities.remove(entity);
        for (auto entity: m_entities)
            entity->update(dt);
    }

    void remove(std::shared_ptr<Entity> entity) {
        m_removeList.remove(entity);
        m_removeList.push_back(entity);
    }

    void reset() {
        m_entities.clear();
    }
    
    std::shared_ptr<Entity> create(std::function<void(std::shared_ptr<Entity>)> func = [](std::shared_ptr<Entity>){}) {
        auto entity = std::make_shared<Entity>();
        m_entities.push_back(entity);
        func(entity);
        return entity;
    }
    
    std::shared_ptr<Entity> find(std::function<bool(const Entity*)> check) {
        for (auto entity: m_entities) {
            if (check(entity.get()))
                return entity;
        }
        return nullptr;
    }
    
    std::list<std::shared_ptr<Entity>> findAll(std::function<bool(const Entity*)> check) {
        std::list<std::shared_ptr<Entity>> list;
        for (auto entity: m_entities) {
            if (check(entity.get()))
                list.push_back(entity);
        }
        return list;
    }
    
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
