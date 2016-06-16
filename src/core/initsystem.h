#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include <entityx/entityx.h>
#include <core/ccamera.h>
#include <core/gpresenter.h>
#include <core/ctransform.h>
#include <core/cbehavoiur.h>

class InitSystem : public entityx::System<InitSystem>
{
public:
    InitSystem();
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entities.each<CBehavoiur>([dt, &entities](entityx::Entity e, CBehavoiur & cBehaviour) {
            cBehaviour.update(e, entities, dt);
        });

        if (first) {
            first = false;

        }
    }
private:
    bool first = true;
};

#endif // INITSYSTEM_H
