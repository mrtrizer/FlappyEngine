#ifndef EntityManager_h
#define EntityManager_h

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include "manager.h"


class Entity;

typedef float TimeDelta;

class Component {
    friend class Entity;
public:
    virtual void update(TimeDelta dt) {
        
    }
    virtual void init() {

    }

    virtual std::size_t getHash() = 0;
    
    inline std::shared_ptr<Entity> getEntity() const { return entity.lock(); }
private:
    std::weak_ptr<Entity> entity;
};

template <typename Dirived>
class BaseComponent: public Component {
public:
    virtual std::size_t getHash() override {return typeid(Dirived).hash_code();}
};

class Entity: public std::enable_shared_from_this<Entity> {
public:
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> add(Args ... args) {
        auto component = std::make_shared<ComponentT>(args...);
        component->entity = shared_from_this();
        component->init();
        components.push_back(component);
        return component;
    }
    
    template<typename ComponentT>
    std::shared_ptr<ComponentT> get() const {
        std::size_t hash = typeid(ComponentT).hash_code();
        for (auto component: components) {
            if (component->getHash() == hash)
                return std::dynamic_pointer_cast<ComponentT>(component);
        }
        return nullptr;
    }
    
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> getAll() const {
        std::size_t hash = typeid(ComponentT).hash_code();
        std::list<std::shared_ptr<ComponentT>> list;
        for (auto component: components) {
            if (component->getHash() == hash)
                list.push_back(std::dynamic_pointer_cast<ComponentT>(component));
        }
        return list;
    }
    
    void update(TimeDelta dt) {
        for (auto component: components)
            component->update(dt);
    }
private:
    std::list<std::shared_ptr<Component>> components;
};

class EntityManager: public Manager<EntityManager> {
public:    
    void update(TimeDelta dt) {
        for (auto entity: entities)
            entity->update(dt);
    }

    void reset() {
        entities.clear();
    }

    static std::shared_ptr<Entity> CREATE(std::function<void(std::shared_ptr<Entity>)> func = [](std::shared_ptr<Entity>){}) {
        getInst()->create(func);
    }
    
    std::shared_ptr<Entity> create(std::function<void(std::shared_ptr<Entity>)> func = [](std::shared_ptr<Entity>){}) {
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


#define E [](std::shared_ptr<Entity> e)
#define CREATE(func) EntityManager::getInst()->create([](std::shared_ptr<Entity> e) func)


#endif /* EntityManager_h */
