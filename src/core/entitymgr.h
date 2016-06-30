#include <list>
#include <map>
#include <typeinfo>
#include <memory>

class Entity;

class Component_ {
public:
    Component_(std::shared_ptr<Entity> entity):entity(entity) {

    }

    virtual void update(float dt) {

    }
private:
    std::weak_ptr<Entity> entity;
};

class Entity: public std::enable_shared_from_this<Entity> {
public:
    template<typename Component>
    std::shared_ptr<Component> add() {
        std::shared_ptr<Component> component = std::make_shared<Component>(shared_from_this());
        components.push_back(component);
        return component;
    }
private:
    std::list<std::shared_ptr<Component>> components;
};

class EntityMgr {
public:
    std::shared_ptr<Entity> create(std::string name) {
        auto entity = std::make_shared<Entity>();
        entities[name] = entity;
        return entity;
    }

    void remove(std::shared_ptr<Entity> entity) {
        //remove
    }

    template <typename Component>
    std::list<std::shared_ptr<Entity>> findAll() {
        std::list<std::shared_ptr<Entity>> entityList;
        //fill entityList
        return entityList;
    }

    template <typename Component>
    std::shared_ptr<Entity> find() {
        auto list = findAll<Component>();
        if (list.size() > 0)
            return list.front();
        else
            return nullptr;
    }

    std::shared_ptr<Entity> findByName(std::string name) {
        //find
        return nullptr;
    }

private:
    std::map<std::string, std::shared_ptr<Entity>> entities;
};
