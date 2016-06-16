#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <entityx/entityx.h>

class Behaviour
{
public:
    Behaviour();
    virtual void update(entityx::Entity, entityx::EntityManager&, entityx::TimeDelta) {
    }
};

#endif // BEHAVIOUR_H
