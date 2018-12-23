#include "Entity.hpp"

#include <Hierarchy.hpp>

namespace flappy {

Handle<Entity> Entity::createEntity() noexcept {
    return m_entities.emplace_back(m_hierarchy->create<Entity>(selfHandle(), m_hierarchy, m_depth + 1));
}

} // flappy
