#include "Entity.h"

#include "ManagerList.h"
#include "TransformComponent.h"
#include "Builder.h"

namespace flappy {

void Entity::update(TimeDelta dt) {
    for (auto component: m_components)
        component->update(dt);
}

std::shared_ptr<Entity> Entity::add(const Builder& builder) {
    if (m_transform == nullptr)
        m_transform = create<TransformComponent>();
    if (auto managerList = m_managerList.lock()) {
        builder.setManagerList(managerList);
        auto entity = managerList->manager<EntityManager>()->create();
        auto transform = entity->create<TransformComponent>();
        builder.build(entity);
        transform->setParent(m_transform);
        return entity;
    } else {
        throw std::runtime_error("ManagerList is destroyed already.");
    }
}

} // flappy
