#pragma once

#include <list>

#include "Component.h"
#include "Manager.h"

namespace flappy {

class EventController;

class Entity: public std::enable_shared_from_this<Entity> {
    friend class TransformComponent;
public:
    struct OnUpdate: public IEvent {
    public:
        OnUpdate(TimeDelta dt): m_dt(dt){}
        TimeDelta dt() const { return m_dt; }
    private:
        TimeDelta m_dt;
    };

    Entity();
    ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    void setParent(std::weak_ptr<Entity> parent);

    std::weak_ptr<Entity> parent();
    std::weak_ptr<Entity> root();

    /// @brief Returns pointer to EventBus.
    std::shared_ptr<EventController> events() {return m_eventController;}

    // Component managment

    /// @brief Useful for manual control of component creation.
    /// @details Use to create components with constructor with arguments.
    /// Also can be used to create multiple components of same type in
    /// single entity.
    /// @see findComponent(), components()
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> createComponent(Args ... args);

    template <typename ComponentT>
    void removeComponent(std::shared_ptr<ComponentT> component);

    /// @brief Search single component of ComponentT type with extra predicate
    /// @details Also looks in children entities with target depth. Don't iterates children
    /// if depth == 0.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> findComponent(std::function<bool(const ComponentT&)> predicate, unsigned depth = 0);

    /// @brief Search single component of ComponentT without predicate
    template<typename ComponentT>
    std::shared_ptr<ComponentT> findComponent(unsigned depth = 0);

    /// @brief Returns first component of required type else creates new component.
    /// @details Useful for default component managment. If only one component
    /// of same type avaliable at same time.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> component();

    /// @brief Returns all components of required type.
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> findComponents(std::function<bool(const ComponentT&)> predicate, unsigned depth = 0) const;

    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> findComponents(unsigned depth = 0) const;

    // Manager managment
    template<typename ManagerT>
    std::shared_ptr<ManagerT> manager();

    template <typename ManagerT, typename ... Args>
    std::shared_ptr<ManagerT> createManager(Args ... args);

    template <typename ManagerT>
    void removeManager(std::shared_ptr<ManagerT> manager);

    template<typename ManagerT>
    std::shared_ptr<ManagerT> findManager(std::function<bool(const ManagerT&)> predicate, unsigned depth = 0);

    template<typename ManagerT>
    std::shared_ptr<ManagerT> findManager(unsigned depth = 0);

    // Entity managment
    std::shared_ptr<Entity> addEntity(std::shared_ptr<Entity> entity = std::make_shared<Entity>());
    void removeEntity(std::shared_ptr<Entity> entity);
    std::shared_ptr<Entity> findEntity(std::function<bool(const Entity&)> predicate, unsigned depth = 0);
    std::list<std::shared_ptr<Entity>> findEntities(std::function<bool(const Entity&)> predicate, unsigned depth = 0);
    std::list<std::shared_ptr<Entity>> findEntities(unsigned depth = 0);

private:
    std::list<std::shared_ptr<Component>> m_components;
    std::list<std::shared_ptr<Entity>> m_entities;
    std::shared_ptr<EventController> m_eventController;
    std::weak_ptr<Entity> m_parent;

    void update(float dt);
};


template <typename ComponentT, typename ... Args>
std::shared_ptr<ComponentT> Entity::createComponent(Args ... args) {
    using namespace std;
    auto component = make_shared<ComponentT>(args...);
    component->setEntity(shared_from_this());
    component->events()->setEventBus(events()->eventBus());
    component->init();
    m_components.push_back(component);
    return component;
}

template <typename ComponentT>
void Entity::removeComponent(std::shared_ptr<ComponentT> component) {
    component->deinit();
    m_components.remove(component);
}

/// @brief Search single component of ComponentT type with extra predicate
/// @details Also looks in children entities with target depth. Don't iterates children
/// if depth == 0.
template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(std::function<bool(const ComponentT&)> predicate, unsigned depth) {
    using namespace std;
    for (auto component: m_components) {
        auto cast = dynamic_pointer_cast<ComponentT>(component);
        if ((cast != nullptr) && predicate(*cast))
            return cast;
    }
    if (depth > 0) {
        for (auto entity: m_entities)
            if (auto component = entity->findComponent<ComponentT>(predicate, depth - 1))
                return component;
    }
    return nullptr;
}

/// @brief Search single component of ComponentT without predicate
template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(unsigned depth) {
    return findComponent<ComponentT>([](const ComponentT&){ return true; }, depth);
}

/// @brief Returns first component of required type else creates new component.
/// @details Useful for default component managment. If only one component
/// of same type avaliable at same time.
template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::component() {
    if (auto foundComponent = findComponent<ComponentT>())
        return foundComponent;
    return createComponent<ComponentT>();
}

/// @brief Returns all components of required type.
template<typename ComponentT>
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(std::function<bool(const ComponentT&)> predicate, unsigned depth) const {
    using namespace std;
    std::list<std::shared_ptr<ComponentT>> list;
    for (auto component: m_components) {
        auto cast = dynamic_pointer_cast<ComponentT>(component);
        if ((cast != nullptr) && predicate(*component))
            list.push_back(dynamic_pointer_cast<ComponentT>(cast));
    }
    if (depth > 0)
        if (auto childResult = findComponents(predicate, depth - 1))
            list.splice(list.end(), childResult);
    return list;
}

template<typename ComponentT>
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(unsigned depth) const {
    return findComponents([](const ComponentT&){ return true; });
}

template<typename ManagerT>
std::shared_ptr<ManagerT> Entity::manager() {
    if (auto manager = findComponent<ManagerT>()) {
        return manager;
    } else {
        if (!parent().expired())
            return parent().lock()->manager<ManagerT>();
        else
            throw std::runtime_error("Manager is unavaliable.");
    }
}

template <typename ManagerT, typename ... Args>
std::shared_ptr<ManagerT> Entity::createManager(Args ... args) {
    return createComponent<ManagerT>(args...);
}

template <typename ManagerT>
void Entity::removeManager(std::shared_ptr<ManagerT> manager) {
    removeComponent(manager);
}

template<typename ManagerT>
std::shared_ptr<ManagerT> Entity::findManager(std::function<bool(const ManagerT&)> predicate, unsigned depth) {
    if (auto manager = findComponent<ManagerT>()) {
        if (predicate(*manager))
            return manager;
    } else {
        if (parent())
            return parent().lock()->manager<ManagerT>(predicate);
        else
            return nullptr;
    }
}

template<typename ManagerT>
std::shared_ptr<ManagerT> Entity::findManager(unsigned depth) {
    findManager<ManagerT>([](const ManagerT&){return true;});
}

} // flappy
