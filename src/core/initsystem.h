#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include <entityx/entityx.h>
#include <core/gobjcamera.h>
#include <core/gpresenter.h>
#include <core/transform.h>
#include <core/cbehavoiur.h>

class InitSystem : public entityx::System<InitSystem>
{
public:
    InitSystem();
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entities.each<CBehavoiur>([dt](entityx::Entity e, CBehavoiur & cBehaviour) {
            cBehaviour.update(e, dt);
        });

        if (first) {
            first = false;

        }
    }
private:
    bool first = true;
};

#endif // INITSYSTEM_H
