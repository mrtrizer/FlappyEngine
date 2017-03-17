#pragma once

#include <list>

#include "Component.h"
#include "Manager.h"

namespace flappy
{

class EventController;

/// Hierarchy of entities is a spine of flappyengine object hierarchy.
/// When Components and Managers are meat, entieis are bones.
class Entity: public std::enable_shared_from_this<Entity>
{
    friend class TransformComponent;
public:
    Entity();
    ~Entity();
    Entity(const Entity&) = delete;
    Entity& operator=(const Entity&) = delete;

    /// Returns parent of current entity or nullptr
    SafePtr<Entity> parent();

    /// Returns first entity in hierarchy
    SafePtr<Entity> root();

    /// @brief Returns pointer to EventBus.
    std::shared_ptr<EventController> events()
    {
        return m_eventController;
    }



    // Component managment

    /// Add component to the list of components.
    /// Entity will be set as component's parent.
    /// Method Component::init() will be called.
    template <typename ComponentT>
    void addComponent(std::shared_ptr<ComponentT> component);

    /// @brief Useful for manual control of component creation.
    /// @details Use to create components with constructor with arguments.
    /// Also can be used to create multiple components of same type in
    /// single entity.
    /// @see findComponent(), components()
    template <typename ComponentT, typename ... Args>
    std::shared_ptr<ComponentT> createComponent(Args ... args);

    /// Remove component from the list of components if exists.
    /// Before changing of component Component::deinit() will be called;
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

    /// Get all components. Works recursively if depth > 0.
    template<typename ComponentT>
    std::list<std::shared_ptr<ComponentT>> findComponents(unsigned depth = 0) const;



    // Manager managment

    /// Add manager to the list of components.
    /// Entity will be set as manager's parent.
    /// Method Component::init() will be called.
    template <typename ManagerT>
    void addManager(std::shared_ptr<ManagerT> manager);

    /// Returns manager from component list of entity.
    /// Throws runtime_error if manager is not found.
    template<typename ManagerT>
    std::shared_ptr<ManagerT> manager();

    /// Create manager and add to the component list
    template <typename ManagerT, typename ... Args>
    std::shared_ptr<ManagerT> createManager(Args ... args);

    /// Remove manager from component list
    template <typename ManagerT>
    void removeManager(std::shared_ptr<ManagerT> manager);



    // Entity managment

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
    std::list<std::shared_ptr<Component>> m_components;
    std::list<std::shared_ptr<Entity>> m_entities;
    std::shared_ptr<EventController> m_eventController;
    SafePtr<Entity> m_parent;

    void setParent(std::weak_ptr<Entity> parent);
};

template<typename ComponentT>
void Entity::addComponent(std::shared_ptr<ComponentT> component)
{
    if (component->entity() != nullptr)
        throw std::runtime_error("Can't add a component to several entities.");
    component->setParentEntity(shared_from_this());
    m_components.push_back(component);
}

template <typename ComponentT, typename ... Args>
std::shared_ptr<ComponentT> Entity::createComponent(Args ... args)
{
    using namespace std;
    auto component = make_shared<ComponentT>(args...);
    addComponent<ComponentT>(component);
    return component;
}

template <typename ComponentT>
void Entity::removeComponent(std::shared_ptr<ComponentT> component)
{
    component->setParentEntity(SafePtr<Entity>());
    m_components.remove(component);
}

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(std::function<bool(const ComponentT&)> predicate, unsigned depth)
{
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

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::findComponent(unsigned depth)
{
    return findComponent<ComponentT>([](const ComponentT&){ return true; }, depth);
}

template<typename ComponentT>
std::shared_ptr<ComponentT> Entity::component()
{
    if (auto foundComponent = findComponent<ComponentT>())
        return foundComponent;
    return createComponent<ComponentT>();
}

template<typename ComponentT>
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(std::function<bool(const ComponentT&)> predicate, unsigned depth) const
{
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
std::list<std::shared_ptr<ComponentT>> Entity::findComponents(unsigned depth) const
{
    return findComponents([](const ComponentT&){ return true; });
}

template<typename ManagerT>
std::shared_ptr<ManagerT> Entity::manager()
{
    if (auto manager = findComponent<ManagerT>())
        return manager;
    else
        throw std::runtime_error("Manager is not added to this entity.");
}

template <typename ManagerT, typename ... Args>
std::shared_ptr<ManagerT> Entity::createManager(Args ... args)
{
    return createComponent<ManagerT>(args...);
}

template <typename ManagerT>
void Entity::removeManager(std::shared_ptr<ManagerT> manager)
{
    removeComponent(manager);
}

} // flappy
