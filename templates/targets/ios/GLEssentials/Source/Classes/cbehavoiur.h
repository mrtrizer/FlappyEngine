#ifndef CBEHAVOIUR_H
#define CBEHAVOIUR_H

#include <memory>

#include <entityx/entityx.h>

#include "component.h"
#include "behaviour.h"

class CBehavoiur: public ComponentSlot<Behaviour>
{
public:
    using ComponentSlot<Behaviour>::ComponentSlot;

    inline void update(entityx::Entity e, entityx::TimeDelta dt) {
        getImpl()->update(e, dt);
    }
};

#endif // CBEHAVOIUR_H
