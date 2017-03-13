#include "SceneManager.h"

#include "TransformComponent.h"

namespace flappy {

using namespace std;

void SceneManager::update(TimeDelta) {
    for (auto entity: m_entities) {
    // TODO: Remove outdate entities
    }
}

shared_ptr<Entity> SceneManager::add(shared_ptr<Entity> entity) {
    m_entities.push_back(entity);
    return entity;
}

} // flappy
