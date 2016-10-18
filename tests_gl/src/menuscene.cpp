#include "menuscene.h"

#include <core/entitymanager.h>
#include <ui/button.h>
#include <core/sprite.h>
#include <core/transform.h>

namespace game {

using namespace flappy;

void MenuScene::init() {
    //Camera
    $(EntityManager)->create([=](EP e){
        e->create<Camera>();
        e->create<Transform>();
    });

    //Background
    EM->create([=](EP e){
        auto sprite = e->create<Sprite>();
        sprite->setPath("img_background");
        auto transform = e->create<Transform>();
        transform->setScale(200);
    });

    $(EntityManager)->create([=](EP e) {
        auto button = e->create<Button>();
        button->setOnClick([]{
            LOGI("Click");
        });
        auto sprite = e->create<Sprite>();
        sprite->setPath("img_play");
        auto transform = e->create<Transform>();
        transform->setScale(10);
    });
}

}
