#include "Entity.h"

#include "EventController.h"
#include "TransformComponent.h"

namespace flappy {

Entity::Entity():
    m_eventController(std::make_shared<EventController>())
{}

Entity::~Entity() {
    for (auto component: m_components)
        component->deinit();
}

void Entity::setParent(std::weak_ptr<Entity> parent) {
    m_parent = parent;
}

std::weak_ptr<Entity> Entity::parent() {
    return m_parent;
}

std::weak_ptr<Entity> Entity::root() {
    auto root = shared_from_this();
    while (!root->parent().expired())
        root = root->parent().lock();
    return root;
}

std::shared_ptr<Entity> Entity::addEntity(std::shared_ptr<Entity> entity) {
    if (!entity->parent().expired() && (entity->parent().lock().get() == this))
        throw std::runtime_error("Can't add same entity twice!");
    entity->setParent(shared_from_this());
    m_entities.push_back(entity);
    if (auto entityManager = manager<SceneManager>())
        entityManager->add(entity);
    return entity;
}

void Entity::removeEntity(std::shared_ptr<Entity> entity) {
    m_entities.remove(entity);
}

std::shared_ptr<Entity> Entity::findEntity(std::function<bool(const Entity&)> predicate, unsigned depth) {
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

std::list<std::shared_ptr<Entity>> Entity::findEntities(std::function<bool(const Entity&)> predicate, unsigned depth) {
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

std::list<std::shared_ptr<Entity>> Entity::findEntities(unsigned depth) {
    if (depth == 0)
        return m_entities;
    else
        return findEntities([](const Entity&){ return true; });
}

void Entity::update(float dt) {
    for (auto component: m_components)
        component->update(dt);
}

} // flappy
