#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <entityx/entityx.h>

class Behaviour
{
public:
    virtual void update(entityx::Entity, entityx::TimeDelta) {
    }
};

#endif // BEHAVIOUR_H
