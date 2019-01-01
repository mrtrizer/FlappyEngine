#include "Entity.hpp"

#include <Hierarchy.hpp>

namespace flappy {

void Entity::removeEntity(const Handle<Entity>& handle) {
    USER_ASSERT_MSG(handle.isValid(), "Invalid entity handle!");
    auto entityIter = std::find_if(m_entities.begin(), m_entities.end(), [&handle](const auto& strongHandle) {
        return handle == strongHandle;
    });

    if (entityIter != m_entities.end())
        m_entities.erase(entityIter);
}

Handle<Entity> Entity::createEntity() {
    return m_entities.emplace_back(m_memoryManager.create<Entity>(selfHandle(), m_hierarchy, m_memoryManager, m_depth + 1));
}

} // flappy
