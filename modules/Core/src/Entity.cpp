#include "Entity.h"

#include "ManagerList.h"
#include "TransformComponent.h"
#include "Builder.h"

namespace flappy {

void Entity::update(TimeDelta dt) {
    for (auto component: m_components)
        component->update(dt);
}

std::shared_ptr<TransformComponent> Entity::transform() {
    if (m_transform == nullptr)
        m_transform = create<TransformComponent>();
    return m_transform;
}

} // flappy
