#include "EntityManager.h"

#include <Builder.h>
#include "TransformComponent.h"

namespace flappy {

using namespace std;

void EntityManager::update(TimeDelta dt) {
    for (auto entity: m_removeList)
        m_entities.remove(entity);
    m_removeList.clear();
    for (auto entity: m_entities)
        entity->update(dt);
}

void EntityManager::remove(shared_ptr<Entity> entity) {
    m_removeList.remove(entity);
    m_removeList.push_back(entity);
}

void EntityManager::reset() {
    m_removeList = m_entities;
}

shared_ptr<Entity> EntityManager::create(function<void(const std::shared_ptr<Entity>&)> func) {
    auto entity = make_shared<Entity>(managerList());
    m_entities.push_back(entity);
    func(entity);
    return entity;
}

shared_ptr<Entity> EntityManager::create() {
    auto entity = make_shared<Entity>(managerList());
    m_entities.push_back(entity);
    return entity;
}

shared_ptr<Entity> EntityManager::add(const Builder &builder) {
    if (auto managerListPtr = managerList().lock()) {
        builder.setManagerList(managerListPtr);
        auto entity = managerListPtr->manager<EntityManager>()->create();
        entity->create<TransformComponent>();
        builder.build(entity);
        return entity;
    } else {
        throw std::runtime_error("ManagerList is destroyed already.");
    }
}

shared_ptr<Entity> EntityManager::find(std::function<bool(const std::shared_ptr<Entity>&)> check) {
    for (auto entity: m_entities) {
        if (check(entity))
            return entity;
    }
    return nullptr;
}

list<shared_ptr<Entity>> EntityManager::findAll(function<bool(const std::shared_ptr<Entity>&)> check) {
    list<shared_ptr<Entity>> list;
    for (auto entity: m_entities) {
        if (check(entity))
            list.push_back(entity);
    }
    return list;
}

} // flappy
