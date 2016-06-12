#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include <entityx/entityx.h>
#include <core/gobjcamera.h>
#include <core/gpresenter.h>
#include <core/gpos.h>

class InitSystem : public entityx::System<InitSystem>
{
public:
    InitSystem();
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entities.each<GPos,GPresenterRect>([dt](entityx::Entity, GPos& pos, GPresenterRect&){
            pos.setX(pos.getX() + 0.1 * dt);
        });
        if (first) {
            first = false;
            auto camera = entities.create();
            camera.assign<GObjCamera>(100, 1, 500);
            auto sprite = entities.create();
            sprite.assign<GPresenterSprite>("background",200, 200);
            sprite.assign<GPos>(-100,-100,0);
            auto rect = entities.create();
            rect.assign<GPresenterRect>(10, 10);
            rect.assign<GPos>(0,0,1);
        }
    }
private:
    bool first = true;
};

#endif // INITSYSTEM_H
