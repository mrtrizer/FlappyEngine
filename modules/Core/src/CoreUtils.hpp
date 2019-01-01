#pragma once

#include <Handle.hpp>
#include <Utility.hpp>

#include "Scheduler.hpp"
#include "Entity.hpp"

namespace flappy::CoreUtils {

inline void removeEntityInstant(const Handle<Entity>& entity) {
    USER_ASSERT_MSG(entity->parent() != nullptr, "Entity has no parent!");
    entity->parent()->removeEntity(entity);
}

inline void removeEntityDelayed(const Handle<Entity>& entity, float delaySec = 0.0f) {
    entity->component<Scheduler>()->schedule(delaySec, [entity]() {
        if (entity.isValid() && entity->parent() != nullptr)
            entity->parent()->removeEntity(entity);
    });
}

} // flappy
