#include "MenuScene.h"

#include <EntityManager.h>
#include <ButtonComponent.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <QuadRes.h>
#include <ResManager.h>

namespace game {

using namespace flappy;

void MenuScene::init() {
    //CameraComponent
    $(EntityManager)->create([=](EP e){
        e->create<CameraComponent>();
        e->create<TransformComponent>();
    });

    //Background
    EM->create([=](EP e){
        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_background"));
        auto transform = e->create<TransformComponent>();
        transform->setScale(2);
    });

    $(EntityManager)->create([=](EP e) {
        auto button = e->create<ButtonComponent>();
        button->setOnClick([]{
            LOGI("Click");
        });
        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_play"));
        auto transform = e->create<TransformComponent>();
        transform->setScale(0.5f);
    });
}

}
