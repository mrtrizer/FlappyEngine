#include "Entity.h"

#include "ManagerList.h"
#include "TransformComponent.h"
#include "Builder.h"

namespace flappy {

void Entity::setManagerList(std::weak_ptr<ManagerList> managerList) {
    if (!m_managerList.expired())
        throw std::runtime_error("You can't add entity to several entity managers.");
    m_managerList = managerList;
    if (auto managerListPtr = m_managerList.lock())
        managerListPtr->events()->eventBus()->addChild(m_eventBus);
    for (auto component: m_components) {
        component->init();
    }
}

void Entity::update(float dt) {
    for (auto component: m_components)
        component->update(dt);
}

std::shared_ptr<Entity> Entity::createEntity() {
    auto entity = managerList().lock()->manager<EntityManager>()->create();
    entity->setParent(shared_from_this());
    return entity;
}

std::shared_ptr<Entity> Entity::addEntity(std::shared_ptr<Entity> entity) {
    managerList().lock()->manager<EntityManager>()->add(entity);
    entity->setParent(shared_from_this());
    return entity;
}

} // flappy
