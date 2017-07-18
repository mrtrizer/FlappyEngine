#pragma once

#include <list>

#include <TypeTraits.h>

#include "Component.h"
#include "Manager.h"

namespace flappy
{

class EventController;
class ComponentBase;

/// Hierarchy of entities is a spine of flappyengine object hierarchy.
/// When Components and Managers are meat, entieis are bones.
class Entity: public std::enable_shared_from_this<Entity>
{
public:
    Entity();
    ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;


    // Component managment

    /// Add component to the list of components.
    /// Entity will be set as component's parent.
    /// Method Component::init() will be called.
    template <typename ComponentT>
    void addComponent(std::shared_ptr<ComponentT> component);

    /// @brief Useful for manual control of component creation.
    /// @details Use to create components with constructor with arguments.
    /// Also can be used to create multiple components of same type in
    /// single entity. Prefere using component<T>() for creation of components with
    /// empty constructors.
    /// @see findComponent(), components()
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> createComponent(Args ... args);

    /// Remove component from the list of components if exists.
    /// Before changing of component method Component::deinit() will be called;
    template <typename ComponentT>
    void removeComponent(std::shared_ptr<ComponentT> component);

    /// @brief Search single component of ComponentT type with extra predicate
    /// @details Also looks in children entities with target depth. Don't iterates children
    /// if depth == 0.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> findComponent(std::function<bool(const ComponentT&)> predicate, unsigned depth = 0);

    /// @brief Search single component of ComponentT without predicate in hierarchy.
    /// @return Returns first match or nullptr.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> findComponent(unsigned depth = 0);

    /// @brief Returns first component of required type else creates new component.
    /// @details Useful for default component managment. If only one component
    /// of same type avaliable at same time. Prefere using this method to create
    /// components without constructors.
    template<typename ComponentT>
    std::shared_ptr<ComponentT> component();

    /// @brief Returns component by id
    std::shared_ptr<ComponentBase> componentById(TypeId<ComponentBase> id);

    /// @brief Returns all components of required type.
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> findComponents(std::function<bool(const ComponentT&)> predicate, unsigned depth = 0) const;

    /// Get all components. Works recursively if depth > 0.
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> findComponents(unsigned depth = 0) const;


    // Entity managment

    /// Returns parent of current entity or nullptr
    SafePtr<Entity> parent();

    /// Returns first entity in hierarchy
    SafePtr<Entity> root();

    /// @brief Returns pointer to EventBus.
    std::shared_ptr<EventController> events()
    {
        return m_eventController;
    }

    /// Add child entity
    void addEntity(std::shared_ptr<Entity> entity);

    /// Create child entity
    std::shared_ptr<Entity> createEntity();

    /// Remove child entity
    void removeEntity(std::shared_ptr<Entity> entity);

    /// Find first match entity with predicate. Works recursive if depth > 0.
    std::shared_ptr<Entity> findEntity(std::function<bool(const Entity&)> predicate, unsigned depth = 0);

    /// Find entities with predicate. Same as findEntity, but returns list of matched entites.
    std::list<std::shared_ptr<Entity>> findEntities(std::function<bool(const Entity&)> predicate, unsigned depth = 0);

    /// Return all children entites. Works recursively if depth > 0.
    std::list<std::shared_ptr<Entity>> findEntities(unsigned depth = 0);

private:
    std::list<std::shared_ptr<ComponentBase>> m_components;
    std::list<std::shared_ptr<Entity>> m_entities;
    std::shared_ptr<EventController> m_eventController;
    TypeMap<ComponentBase, SafePtr<ManagerBase>> m_managers;
    SafePtr<Entity> m_parent;

    void setParent(SafePtr<Entity> parent);

    template <typename ManagerEventT>
    void sendManagerEvents(std::shared_ptr<EventController> eventController);

    template<typename ComponentT, typename ComponentEventT>
    void sendComponentEvents(std::shared_ptr<ComponentT> component);
};


template <typename ManagerEventT>
void Entity::sendManagerEvents(std::shared_ptr<EventController> eventController) {
    for (int i = 0; i < m_managers.size(); i++) {
        TypeId<ComponentBase> id(i);
        auto manager = m_managers.getById(id);
        if (manager != nullptr) {
            auto event = ManagerEventT();
            event.id = id;
            event.pointer = manager;
            eventController->post(event);
        }
    }
}

template<typename ComponentT, typename ComponentEventT>
void Entity::sendComponentEvents(std::shared_ptr<ComponentT> component) {
    for (auto currentComponent : m_components) {
        if (currentComponent != component) {
            // Notify already added components
            ComponentBase::ComponentAddedEvent newComponentEvent;
            newComponentEvent.id = component->componentId();
            newComponentEvent.pointer = component;
            currentComponent->events()->post(newComponentEvent);
            // Notify new component
            ComponentBase::ComponentAddedEvent oldComponentEvent;
            oldComponentEvent.id = currentComponent->componentId();
            oldComponentEvent.pointer = currentComponent;
            component->events()->post(oldComponentEvent);
        }
    }

}

// Component managment

template<typename ComponentT>
void Entity::addComponent(std::shared_ptr<ComponentT> component)
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    if (component->entity() != nullptr)
        throw std::runtime_error("Can't add a component to several entities.");
    component->setParentEntity(shared_from_this());
    m_components.push_back(component);
    // Notify component about accesible managers
    sendManagerEvents<ManagerBase::ManagerAddedEvent>(component->events());

    // Notify components about new component
    sendComponentEvents<ComponentT, ComponentBase::ComponentAddedEvent>(component);
}

template <typename ComponentT, typename ... Args>
std::shared_ptr<ComponentT> Entity::createComponent(Args ... args)
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    auto component = std::make_shared<ComponentT>(args...);
    addComponent<ComponentT>(component);
    return component;
}

template <typename ComponentT>
void Entity::removeComponent(std::shared_ptr<ComponentT> component)
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    // Notify component about losing of all managers
    sendManagerEvents<ManagerBase::ManagerRemovedEvent>(component->events());

    // Notify components about component removing
    sendComponentEvents<ComponentT, ComponentBase::ComponentRemovedEvent>(component);

    component->setParentEntity(SafePtr<Entity>());

    m_components.remove(component);
}

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(std::function<bool(const ComponentT&)> predicate, unsigned depth)
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    for (auto component: m_components) {
        auto cast = std::dynamic_pointer_cast<ComponentT>(component);
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

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(unsigned depth)
{
    return findComponent<ComponentT>([](const ComponentT&){ return true; }, depth);
}

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::component()
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    if (auto foundComponent = findComponent<ComponentT>())
        return foundComponent;
    return createComponent<ComponentT>();
}

template<typename ComponentT>
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(std::function<bool(const ComponentT&)> predicate, unsigned depth) const
{
    static_assert(isBaseOf<ComponentBase, ComponentT>(), "Type must be a descendant of Component");

    std::list<std::shared_ptr<ComponentT>> list;
    for (auto component: m_components) {
        auto cast = std::dynamic_pointer_cast<ComponentT>(component);
        if ((cast != nullptr) && predicate(*cast))
            list.push_back(cast);
    }
    if (depth > 0) {
        for (auto entity: m_entities) {
            auto childResult = entity->findComponents<ComponentT>(predicate, depth - 1);
            if (!childResult.empty())
                list.splice(list.end(), childResult);
        }
    }
    return list;
}

template<typename ComponentT>
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(unsigned depth) const
{
    return findComponents<ComponentT>([](const ComponentT&){ return true; }, depth);
}

} // flappy
