#ifndef PREFAB_H
#define PREFAB_H

#include <entityx/entityx.h>

class Prefab
{
public:
    virtual void init(entityx::EntityManager& entityManager) = 0;
};

#endif // PREFAB_H
