#include "Entity.h"

#include "EventController.h"
#include "ComponentBase.h"

namespace flappy
{

Entity::Entity():
    m_eventController(std::make_shared<EventController>())
{
    events()->subscribe([this](const ComponentBase::ManagerAddedEvent& e) {
        m_managers.setById(e.id, e.pointer);
    });
    events()->subscribe([this](const ComponentBase::ManagerRemovedEvent& e) {
        m_managers.setById(e.id, SafePtr<ManagerBase>());
    });

    events()->subscribeAll([this](const EventHandle& handle) {
        for (auto component: m_components) {
            component->events()->eventBus()->post(handle);
        }
        for (auto entity: m_entities) {
            entity->events()->eventBus()->post(handle);
        }
    });

}

Entity::~Entity() {
    for (auto component : m_components) {
        component->setActive(false);
    }
}

void Entity::addComponent(std::shared_ptr<ComponentBase> component)
{
    if (component->entity() != nullptr)
        throw std::runtime_error("Can't add a component to several entities.");
    component->setParentEntity(this, shared_from_this());
    m_components.push_back(component);
    // Notify component about accesible managers
    sendManagerEvents<ManagerBase::ManagerAddedEvent>(component->events());

    // Notify components about new component
    sendComponentEvents<ComponentBase::ComponentAddedEvent>(component);
}

std::shared_ptr<ComponentBase> Entity::componentById(TypeId<ComponentBase> id) {
    for (auto component: m_components)
        if (component->componentId() == id)
            return component;
    return nullptr;
}

void Entity::setParent(SafePtr<Entity> parent)
{
    m_parent = parent;
}

SafePtr<Entity> Entity::parent()
{
    return m_parent;
}

SafePtr<Entity> Entity::root()
{
    auto root = SafePtr<Entity>(shared_from_this());
    while (root->parent())
        root = root->parent();
    return root;
}

void Entity::addEntity(std::shared_ptr<Entity> entity)
{
    if (entity->parent() && (entity->parent() == this))
        throw std::runtime_error("Can't add same entity twice!");
    entity->setParent(shared_from_this());
    m_entities.push_back(entity);
    sendManagerEvents<ComponentBase::ManagerAddedEvent>(entity->events());
}

std::shared_ptr<Entity> Entity::createEntity()
{
    auto entity = std::make_shared<Entity>();
    addEntity(entity);
    return entity;
}

void Entity::removeEntity(std::shared_ptr<Entity> entity)
{
    sendManagerEvents<ComponentBase::ManagerRemovedEvent>(entity->events());
    m_entities.remove(entity);
}

std::shared_ptr<Entity> Entity::findEntity(std::function<bool(const Entity&)> predicate, unsigned depth)
{
    for (auto entity: m_entities) {
        if (predicate(*entity)) {
            return entity;
        } else {
            if (depth > 0)
                if (auto childResult = entity->findEntity(predicate, depth - 1))
                    return childResult;
        }
    }
    return nullptr;
}

std::list<std::shared_ptr<Entity>> Entity::findEntities(std::function<bool(const Entity&)> predicate, unsigned depth)
{
    std::list<std::shared_ptr<Entity>> list;
    for (auto entity: m_entities) {
        if (predicate(*entity))
            list.push_back(entity);
        if (depth > 0) {
            auto childResult = entity->findEntities(predicate, depth - 1);
            if (!childResult.empty())
                list.splice(list.end(), childResult);
        }
    }
    return list;
}

std::list<std::shared_ptr<Entity>> Entity::findEntities(unsigned depth)
{
    if (depth == 0)
        return m_entities;
    else
        return findEntities([](const Entity&){ return true; }, depth);
}

} // flappy
