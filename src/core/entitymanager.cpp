#include "entitymanager.h"

template <> bool EntityManager::check <void> (std::shared_ptr<Entity> entity) {
    return true;
}
