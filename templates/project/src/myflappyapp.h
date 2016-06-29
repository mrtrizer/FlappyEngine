#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <core/scenemanager.h>
#include <core/cpresenter.h>
#include <core/ctransform.h>
#include <core/cbehavoiur.h>
#include <core/animation.h>
#include <core/flappyapp.h>

class Rotate: public Behaviour {
public:
    void update(entityx::Entity e, entityx::TimeDelta dt) {
        e.component<CTransform>()->angle += dt;
    }
};


class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        SceneManager::createEntity().add<CCamera>();
        //Background
        auto sprite = SceneManager::createEntity();
        sprite.add<CPresenter,GPresenterSprite>("bird",30, 30, 2);
        sprite.add<CTransform>();
        sprite.add<CBehavoiur, Rotate>();
        
    }
};

#endif // MYFLAPPYAPP_H
