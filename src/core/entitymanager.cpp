#include "entitymanager.h"

template <> bool EntityMgr::check <void> (std::shared_ptr<Entity>) {
    return true;
}

namespace EM {

void create(std::function<void(std::shared_ptr<Entity>)> func) {
    EntityMgr::getInst()->create(func);
}

void remove(std::shared_ptr<Entity> entity) {
    EntityMgr::getInst()->remove(entity);
}

std::list<std::shared_ptr<Entity>> findall(std::function<bool(const Entity*)> check) {
    return EntityMgr::getInst()->findAll(check);
}

std::shared_ptr<Entity> find(std::function<bool(const Entity*)> check) {
    return EntityMgr::getInst()->find(check);
}

}
