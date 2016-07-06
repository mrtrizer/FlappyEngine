#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <core/scenemanager.h>
#include <core/ctransform.h>
#include <core/animation.h>
#include <core/flappyapp.h>
#include <core/entitymanager.h>
#include <core/inputmanager.h>

class Rotate: public BaseComponent<Rotate> {
public:
    void update(TimeDelta dt) {
        getEntity()->get<CTransform>()->angle += dt * getAngle();
    }

    virtual float getAngle() {
        return 1.0f;
    }
};

class RotateRight: public Rotate {
public:
    float getAngle() {
        return -2.0f;
    }
};

class Move: public BaseComponent<Move> {
public:
    void update(TimeDelta dt) {
        getEntity()->get<CTransform>()->pos =
                SceneManager::getInst()->getCamera()->screenToScene(InputManager::getInst()->getMousePos());
        //Scene::getCamera()->screenToScene(Input::getMousePos());
    }
};


class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        CREATE({
                    e->add<CCamera>();
               });

        //Background
        CREATE({
                    e->add<GPresenterSprite>("background",200, 200, 1);
                    e->add<CTransform>();
                    e->add<RotateRight>();
               });

        //Bird
        CREATE({
                    e->add<GPresenterSprite>("bird",30, 30, 2);
                    e->add<CTransform>();
                    e->add<Rotate>();
                    e->add<Move>();
               });
        
    }
};

#endif // MYFLAPPYAPP_H
