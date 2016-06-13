#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <entityx/entityx.h>

class Behaviour
{
public:
    Behaviour();
    virtual void update(entityx::Entity e, entityx::TimeDelta dt) {
    }
};

#endif // BEHAVIOUR_H
