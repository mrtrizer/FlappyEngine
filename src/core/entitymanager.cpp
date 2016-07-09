#include "entitymanager.h"

template <> bool EntityManager::check <void> (std::shared_ptr<Entity>) {
    return true;
}

void CREATE(std::function<void(std::shared_ptr<Entity>)> func) {
    EntityManager::getInst()->create(func);
}
