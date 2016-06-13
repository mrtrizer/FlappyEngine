#ifndef INITSYSTEM_H
#define INITSYSTEM_H

#include <entityx/entityx.h>
#include <core/gobjcamera.h>
#include <core/gpresenter.h>
#include <core/transform.h>

class InitSystem : public entityx::System<InitSystem>
{
public:
    InitSystem();
    void update(entityx::EntityManager &entities, entityx::EventManager &events, entityx::TimeDelta dt) override {
        entities.each<GPresenterRect, Transform>([](entityx::Entity e, GPresenterRect& rect, Transform& transform) {
            transform.angle += 0.01;
        });

        if (first) {
            first = false;
            auto camera = entities.create();
            camera.assign<GObjCamera>(100, 1, 500);
            auto sprite = entities.create();
            sprite.assign<GPresenterSprite>("background",200, 200);
            auto transform = sprite.assign<Transform>(-100,-100,0);
            auto rect = entities.create();
            rect.assign<GPresenterRect>(10, 10);
            rect.assign<Transform>(0,0,1);
        }
    }
private:
    bool first = true;
};

#endif // INITSYSTEM_H
