#include "Hierarchy.hpp"

#include "Entity.hpp"

void Hierarchy::updateEntity(const StrongHandle<Entity>& entity, float dt) {
    for (auto updateFunction : entity->updateFunctions())
        updateFunction(dt);
    for (const auto& subEntity : entity->children())
        updateEntity(subEntity, dt);
}
