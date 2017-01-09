#include "GameScene.h"
#include <AtlasRes.h>
#include <ResManager.h>
#include <SpriteComponent.h>
#include <PresenterComponent.h>
#include <QuadRes.h>
#include <SizeComponent.h>
#include <MenuScene.h>

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameScene::createBasket(string color, vec2 pos) {
    manager<EntityManager>()->create([=](EP e) {
        auto quad = manager<ResManager>()->getRes<QuadRes>("img_baskets:" + color);

        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(quad);

        auto sizeComponent = e->create<SizeComponent>();
        sizeComponent->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));

        auto transform = e->create<TransformComponent>();
        transform->setPos(vec3(pos, 0));
        transform->setScale(1);

        e->create<BasketCtrl>()->setColor(color);
    });
}

void GameScene::init() {
    auto atlas = std::make_shared<AtlasRes>();
    atlas->addSpriteInfo("blue",AtlasRes::SpriteInfo({0,0,0.333f,1},{100, 100}));
    atlas->addSpriteInfo("green",AtlasRes::SpriteInfo({0.333f,0,0.333f * 2.0f,1.0f},{100, 100}));
    atlas->addSpriteInfo("red",AtlasRes::SpriteInfo({0.333f * 2.0f,0,0.333 * 3.0f,1.0f},{100, 100}));
    manager<ResManager>()->setRes<AtlasRes>("img_baskets", atlas);

    //CameraComponent
    manager<EntityManager>()->create([=](EP e){
        auto camera = e->create<CameraComponent>();
        camera->setHeight(640);
        setCamera(camera);
    });

    //Game controller
    manager<EntityManager>()->create([=](EP e){
        e->create<GameCtrl>();
    });

//    //Background
//    manager<EntityManager>()->create([=](EP e){
//        auto sprite = e->create<SpriteComponent>();
//        sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_background"));
//        auto transform = e->create<TransformComponent>();
//        transform->setScale(2);
//    });

    //Baskets
    createBasket("blue", {0, 250});
    createBasket("red", {-200, 250});
    createBasket("green", {200, 250});

    //Menu button
    manager<EntityManager>()->create([=](EP e) {
        auto buttonComponent = e->create<ButtonComponent>();
        events()->subscribe(buttonComponent, [this](ButtonComponent::OnButtonClick e) {
            LOGI("Click");

            auto menuScene = std::make_shared<MenuScene>();
            manager<SceneManager>()->setScene(menuScene);
        });
        auto quad = manager<ResManager>()->getRes<QuadRes>("img_play");
        e->create<SpriteComponent>()
                ->setQuad(quad);
        auto transform = e->create<TransformComponent>();
        transform->setScale(0.5f);
        transform->setPos({0, -260, 0});
        e->create<SizeComponent>()
                ->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));
    });
}

} // flappy
