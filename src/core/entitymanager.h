//
//  EntityManager.h
//  ComponentManager
//
//  Created by Denis Zdorovtsov on 05.07.16.
//  Copyright © 2016 Denis Zdorovtsov. All rights reserved.
//

#ifndef EntityManager_h
#define EntityManager_h

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

class Entity;

class Component {
    friend class Entity;
public:
    virtual void update(float dt) {
        
    }
    
    inline std::shared_ptr<Entity> getEntity() const { return entity.lock(); }
private:
    std::weak_ptr<Entity> entity;
};

class Entity: public std::enable_shared_from_this<Entity> {
public:
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> add(Args ... args) {
        auto component = std::make_shared<ComponentT>(args...);
        component->entity = shared_from_this();
        components.push_back(component);
        return component;
    }
    
    template<typename ComponentT>
    std::shared_ptr<ComponentT> get() const {
        std::size_t hash = typeid(ComponentT).hash_code();
        for (auto component: components) {
            Component& curComponent = *component.get();
            if (typeid(curComponent).hash_code() == hash)
                return std::dynamic_pointer_cast<ComponentT>(component);
        }
        return nullptr;
    }
    
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> getAll() const {
        std::size_t hash = typeid(ComponentT).hash_code();
        std::list<std::shared_ptr<ComponentT>> list;
        for (auto component: components) {
            Component& curComponent = *component.get();
            if (typeid(curComponent).hash_code() == hash)
                list.push_back(std::dynamic_pointer_cast<ComponentT>(component));
        }
        return list;
    }
    
    void update(float dt) {
        for (auto component: components)
            component->update(dt);
    }
private:
    std::list<std::shared_ptr<Component>> components;
};

class EntityMgr {
public:
    void update(float dt) {
        for (auto entity: entities)
            entity->update(dt);
    }
    
    std::shared_ptr<Entity> create() {
        auto entity = std::make_shared<Entity>();
        entities.push_back(entity);
        return entity;
    }
    
    std::shared_ptr<Entity> create(std::function<void(std::shared_ptr<Entity>)> func) {
        auto entity = std::make_shared<Entity>();
        entities.push_back(entity);
        func(entity);
        return entity;
    }
    
    std::shared_ptr<Entity> find(std::function<bool(const Entity*)> check) {
        for (auto entity: entities) {
            if (check(entity.get()))
                return entity;
        }
        return nullptr;
    }
    
    std::list<std::shared_ptr<Entity>> findAll(std::function<bool(const Entity*)> check) {
        std::list<std::shared_ptr<Entity>> list;
        for (auto entity: entities) {
            if (check(entity.get()))
                list.push_back(entity);
        }
        return list;
    }
    
    template <typename ... Components>
    void each(std::function<void(std::shared_ptr<Entity>)> func) {
        for (auto entity: entities) {
            if (check<Components...>(entity))
                func(entity);
        }
    }
private:
    std::list<std::shared_ptr<Entity>> entities;
    
    template <typename ComponentT = void, typename ... Components>
    bool check(std::shared_ptr<Entity> entity) {
        return check<Components...>(entity) && (entity->get<ComponentT>() != nullptr);
    }
};

template <> bool EntityMgr::check <void> (std::shared_ptr<Entity> entity) {
    return true;
}

#define CREATE(func) entityMgr.create([](std::shared_ptr<Entity> e) func)
#define FIND(func) entityMgr.find([](const Entity* e) func)
#define EACH(func,...) entityMgr.each <__VA_ARGS__> ([](std::shared_ptr<Entity> entity) func);

#endif /* EntityManager_h */
