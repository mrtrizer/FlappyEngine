#include "entitymanager.h"

template <> bool EntityManager::check <void> (std::shared_ptr<Entity>) {
    return true;
}

void CREATE(std::function<void(std::shared_ptr<Entity>)> func) {
    EntityManager::getInst()->create(func);
}

void REMOVE(std::shared_ptr<Entity> entity) {
    EntityManager::getInst()->remove(entity);
}

std::list<std::shared_ptr<Entity>> FINDALL(std::function<bool(const Entity*)> check) {
    return EntityManager::getInst()->findAll(check);
}

std::shared_ptr<Entity> FIND(std::function<bool(const Entity*)> check) {
    return EntityManager::getInst()->find(check);
}
