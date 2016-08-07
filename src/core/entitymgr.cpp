#include "entitymgr.h"

namespace flappy {

template <> bool EntityMgr::check <void> (shared_ptr<Entity>) {
    return true;
}

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
    m_entities.clear();
}

shared_ptr<Entity> EntityMgr::create(function<void(shared_ptr<Entity>)> func) {
    auto entity = make_shared<Entity>(flappyApp());
    m_entities.push_back(entity);
    func(entity);
    return entity;
}

shared_ptr<Entity> EntityMgr::find(function<bool(const Entity*)> check) {
    for (auto entity: m_entities) {
        if (check(entity.get()))
            return entity;
    }
    return nullptr;
}

list<shared_ptr<Entity>> EntityMgr::findAll(function<bool(const Entity*)> check) {
    list<shared_ptr<Entity>> list;
    for (auto entity: m_entities) {
        if (check(entity.get()))
            list.push_back(entity);
    }
    return list;
}

} // flappy
