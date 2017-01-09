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
//    auto camera = manager<EntityManager>()->create<CameraEntity>(640, 960);
//    setCamera(camera);

//    manager<EntityManager>()->create<SpriteEntity>("img_background");

//    manager<EntityManager>()->create<ButtonEntity>("img_play_up", "img_play_down");

    //CameraComponent
    manager<EntityManager>()->create([=, this](EP e){
        auto camera = e->create<CameraComponent>();
        camera->setHeight(640);
        setCamera(camera);
        e->create<TransformComponent>();
    });

    //Background
    manager<EntityManager>()->create([=](EP e){
        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(manager<ResManager>()->getRes<QuadRes>("img_background"));
        auto transform = e->create<TransformComponent>();
        transform->setScale(2);
    });

    manager<EntityManager>()->create([=](EP e) {
        auto buttonComponent = e->create<ButtonComponent>();
        events()->subscribe(buttonComponent, [this](ButtonComponent::OnButtonClick e) {
            LOGI("Click");

            auto gameScene = std::make_shared<GameScene>();
            manager<SceneManager>()->setScene(gameScene);
        });
        auto quad = manager<ResManager>()->getRes<QuadRes>("img_play");
        e->create<SpriteComponent>()
                ->setQuad(quad);
        e->create<TransformComponent>()
                ->setScale(0.5f);
        e->create<SizeComponent>()
                ->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));
    });
}

}
