#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <core/scenemanager.h>
#include <core/cpresenter.h>
#include <core/ctransform.h>
#include <core/cbehavoiur.h>
#include <core/animation.h>
#include <core/flappyapp.h>

class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        SceneManager::createEntity().add<CCamera>();
        //Background
        auto sprite = SceneManager::createEntity();
        sprite.add<CPresenter,GPresenterRect>(10, 10);
    }
};

#endif // MYFLAPPYAPP_H
