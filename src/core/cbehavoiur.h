#ifndef CBEHAVOIUR_H
#define CBEHAVOIUR_H

#include <memory>

#include <entityx/entityx.h>

#include "behaviour.h"

class CBehavoiur
{
public:
    CBehavoiur();
    void setBehaviour(const std::shared_ptr<Behaviour> & behaviour) {
        this->behaviour = behaviour;
    }

    void update(entityx::Entity e, entityx::EntityManager& em, entityx::TimeDelta dt) {
        behaviour->update(e, em, dt);
    }

private:
    std::shared_ptr<Behaviour> behaviour;
};

#endif // CBEHAVOIUR_H
