#include "entitymgr.h"

namespace flappy {

using namespace std;

void EntityMgr::update(TimeDelta dt) {
    for (auto entity: m_removeList)
        m_entities.remove(entity);
    for (auto entity: m_entities)
        entity->update(dt);
}

void EntityMgr::remove(shared_ptr<Entity> entity) {
    m_removeList.remove(entity);
    m_removeList.push_back(entity);
}

void EntityMgr::reset() {
    m_removeList = m_entities;
}

shared_ptr<Entity> EntityMgr::create(function<void(const std::shared_ptr<Entity>&)> func) {
    auto entity = make_shared<Entity>(managerList());
    m_entities.push_back(entity);
    func(entity);
    return entity;
}

shared_ptr<Entity> EntityMgr::create() {
    auto entity = make_shared<Entity>(managerList());
    m_entities.push_back(entity);
    return entity;
}

shared_ptr<Entity> EntityMgr::find(std::function<bool(const std::shared_ptr<Entity>&)> check) {
    for (auto entity: m_entities) {
        if (check(entity))
            return entity;
    }
    return nullptr;
}

list<shared_ptr<Entity>> EntityMgr::findAll(function<bool(const std::shared_ptr<Entity>&)> check) {
    list<shared_ptr<Entity>> list;
    for (auto entity: m_entities) {
        if (check(entity))
            list.push_back(entity);
    }
    return list;
}

} // flappy
