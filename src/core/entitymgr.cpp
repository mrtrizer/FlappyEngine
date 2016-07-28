#include "entitymgr.h"

template <> bool EntityMgr::check <void> (std::shared_ptr<Entity>) {
    return true;
}

void EntityMgr::update(TimeDelta dt) {
    for (auto entity: m_removeList)
        m_entities.remove(entity);
    for (auto entity: m_entities)
        entity->update(dt);
}

void EntityMgr::remove(std::shared_ptr<Entity> entity) {
    m_removeList.remove(entity);
    m_removeList.push_back(entity);
}

void EntityMgr::reset() {
    m_entities.clear();
}

std::shared_ptr<Entity> EntityMgr::create(std::function<void(std::shared_ptr<Entity>)> func) {
    auto entity = std::make_shared<Entity>();
    m_entities.push_back(entity);
    func(entity);
    return entity;
}

std::shared_ptr<Entity> EntityMgr::find(std::function<bool(const Entity*)> check) {
    for (auto entity: m_entities) {
        if (check(entity.get()))
            return entity;
    }
    return nullptr;
}

std::list<std::shared_ptr<Entity>> EntityMgr::findAll(std::function<bool(const Entity*)> check) {
    std::list<std::shared_ptr<Entity>> list;
    for (auto entity: m_entities) {
        if (check(entity.get()))
            list.push_back(entity);
    }
    return list;
}
