#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <prefabs/camera.h>
#include <core/cpresenter.h>
#include <core/ctransform.h>
#include <core/cbehavoiur.h>
#include <core/animation.h>
#include <core/flappyapp.h>

class Rotate: public Behaviour {
public:
    void update(entityx::Entity e, entityx::TimeDelta dt) {
        e.component<CTransform>()->angle += 0.1 * dt;
    }
};

class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        initPrefab(Camera());
        //Background
        auto sprite = entities.create();
        sprite.assign<CPresenter>()->setPresenter(std::make_shared<GPresenterSprite>("background",200, 200));
        auto transform = sprite.assign<CTransform>(-100,-100,0);
        //Bird
        auto rect = entities.create();
        rect.assign<CPresenter>()->setPresenter(std::make_shared<SpriteAnimation>("bird", 10, 10, 2, 0.5f));
        rect.assign<CTransform>(0,0,1);
        rect.assign<CBehavoiur>()->setBehaviour(std::make_shared<Rotate>());
    }
};

#endif // MYFLAPPYAPP_H
