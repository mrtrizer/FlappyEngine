#include "Entity.h"

#include "EventController.h"

namespace flappy
{

Entity::Entity():
    m_eventController(std::make_shared<EventController>())
{
    events()->subscribeIn([this](const AManager::OnManagerAdded& e) {
        m_managers.setById(e.id, e.pointer);
    });
    events()->subscribeIn([this](const AManager::OnManagerRemoved& e) {
        m_managers.setById(e.id, SafePtr<AManager>());
    });

    events()->subscribeInAll([this](const EventHandle& handle) {
        FlowStatus flowStatus = FlowStatus::CONTINUE;
        for (auto component: m_components) {
            auto curFlowStatus = component->events()->eventBus()->post(handle);
            if (curFlowStatus != FlowStatus::CONTINUE)
                flowStatus = curFlowStatus;
        }
        for (auto entity: m_entities) {
            auto curFlowStatus = entity->events()->eventBus()->post(handle);
            if (curFlowStatus != FlowStatus::CONTINUE)
                flowStatus = curFlowStatus;
        }
        return flowStatus;
    });

}

Entity::~Entity()
{
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
    sendManagerEvents<AManager::OnManagerAdded>(entity->events());
}

std::shared_ptr<Entity> Entity::createEntity()
{
    auto entity = std::make_shared<Entity>();
    addEntity(entity);
    return entity;
}

void Entity::removeEntity(std::shared_ptr<Entity> entity)
{
    sendManagerEvents<AManager::OnManagerRemoved>(entity->events());
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
