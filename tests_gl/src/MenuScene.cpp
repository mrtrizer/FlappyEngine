#include "MenuScene.h"

#include <EntityManager.h>
#include <ButtonComponent.h>
#include <SpriteComponent.h>
#include <TransformComponent.h>
#include <QuadRes.h>
#include <ResManager.h>
#include <SizeComponent.h>

#include "GameScene.h"

namespace game {

using namespace flappy;

void MenuScene::init() {
    //CameraComponent
    EM->create([=](EP e){
        auto camera = e->create<CameraComponent>();
        camera->setHeight(640);
        e->create<TransformComponent>();
    });

    //Background
    EM->create([=](EP e){
        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_background"));
        auto transform = e->create<TransformComponent>();
        transform->setScale(2);
    });

    EM->create([=](EP e) {
        auto buttonComponent = e->create<ButtonComponent>();
        events()->subscribe(buttonComponent, [this](ButtonComponent::OnButtonClick e) {
            LOGI("Click");

            auto gameScene = std::make_shared<GameScene>();
            MGR<SceneManager>()->setScene(gameScene);
        });
        auto quad = MGR<ResManager>()->getResSync<QuadRes>("img_play");
        e->create<SpriteComponent>()
                ->setQuad(quad);
        e->create<TransformComponent>()
                ->setScale(0.5f);
        e->create<SizeComponent>()
                ->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));
    });
}

}
