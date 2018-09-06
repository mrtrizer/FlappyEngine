#include "Hierarchy.hpp"

#include "Entity.hpp"

namespace flappy {

void Hierarchy::updateEntity(const Handle<Entity>& entity, float dt) {
    for (auto updateFunction : entity->updateFunctions())
        updateFunction(dt);
    for (const auto& subEntity : entity->entities())
        updateEntity(subEntity, dt);
}

} // flappy
