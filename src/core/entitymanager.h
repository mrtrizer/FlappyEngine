#ifndef EntityManager_h
#define EntityManager_h

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include "manager.h"

class Transform;
class Entity;

typedef float TimeDelta;

class Component {
    friend class Entity;
public:
    virtual void update(TimeDelta) {
        
    }
    virtual void init() {

    }

    virtual std::size_t getHash() = 0;
    
    inline std::shared_ptr<Entity> entity() const { return _entity.lock(); }
private:
    std::weak_ptr<Entity> _entity;
};

template <typename Dirived>
class BaseComponent: public Component {
public:
    virtual std::size_t getHash() override {return typeid(Dirived).hash_code();}
};

class Entity: public std::enable_shared_from_this<Entity> {
    friend class Transform;
public:
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> add(Args ... args) {
        auto component = std::make_shared<ComponentT>(args...);
        component->_entity = shared_from_this();
        component->init();
        components.push_back(component);
        return component;
    }
    
    //TODO: How to optomize? Dynamic cast for every component is bad idea.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> get() const {
        for (auto component: components) {
            auto cast = std::dynamic_pointer_cast<ComponentT>(component);
            if (cast != nullptr)
                return cast;
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

    std::shared_ptr<Transform> transform() { return _transform; }

private:
    std::list<std::shared_ptr<Component>> components;
    std::shared_ptr<Transform> _transform;
};

class EntityManager: public Manager<EntityManager> {
public:    
    void update(TimeDelta dt) {
        for (auto entity: removeList)
            entities.remove(entity);
        for (auto entity: entities)
            entity->update(dt);
    }

    void remove(std::shared_ptr<Entity> entity) {
        removeList.remove(entity);
        removeList.push_back(entity);
    }

    void reset() {
        entities.clear();
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
    std::list<std::shared_ptr<Entity>> removeList;
    
    template <typename ComponentT = void, typename ... Components>
    bool check(std::shared_ptr<Entity> entity) {
        return check<Components...>(entity) && (entity->get<ComponentT>() != nullptr);
    }
};


typedef std::shared_ptr<Entity> EP;
void CREATE(std::function<void(std::shared_ptr<Entity>)> func);
void REMOVE(std::shared_ptr<Entity> entity);
std::list<std::shared_ptr<Entity>> FINDALL(std::function<bool(const Entity*)> check);
std::shared_ptr<Entity> FIND(std::function<bool(const Entity*)> check);
template <typename ... Components>
void EACH(std::function<void(std::shared_ptr<Entity>)> func) {
    EntityManager::getInst()->each<Components...>(func);
}

#endif /* EntityManager_h */
