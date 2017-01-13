#include "GameScene.h"
#include <AtlasRes.h>
#include <ResManager.h>
#include <SpriteComponent.h>
#include <PresenterComponent.h>
#include <QuadRes.h>
#include <SizeComponent.h>
#include <SpriteBuilder.h>
#include <CameraBuilder.h>
#include <ButtonBuilder.h>

#include "MenuScene.h"

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameScene::createBasket(string color, vec2 pos) {

    auto quad = manager<ResManager>()->getRes<QuadRes>("img_baskets:" + color);

    auto basket = manager<EntityManager>()->create();

    auto sprite = basket->component<SpriteComponent>();
    sprite->setQuad(quad);

    auto sizeComponent = basket->component<SizeComponent>();
    sizeComponent->setSize(glm::vec3(quad->spriteInfo().size, 0.0f));

    auto transform = basket->component<TransformComponent>();
    transform->setPos(vec3(pos, 0));

    basket->create<BasketCtrl>()->setColor(color);
}

void GameScene::init() {
    auto atlas = std::make_shared<AtlasRes>();
    atlas->addSpriteInfo("blue",AtlasRes::SpriteInfo({0,0,0.333f,1},{100, 100}));
    atlas->addSpriteInfo("green",AtlasRes::SpriteInfo({0.333f,0,0.333f * 2.0f,1.0f},{100, 100}));
        atlas->addSpriteInfo("red",AtlasRes::SpriteInfo({0.333f * 2.0f,0,0.333 * 3.0f,1.0f},{100, 100}));
    manager<ResManager>()->setRes<AtlasRes>("img_baskets", atlas);

    //Camera
    auto camera = manager<EntityManager>()->create();
    setCamera(camera->component<CameraComponent>()
              ->setSize({960, 640}));

//    //Game controller
//    manager<EntityManager>()->create([=](EP e){
//        e->create<GameCtrl>();
//    });

    //Background
    auto sprite = manager<EntityManager>()->create();
    sprite->component<SpriteComponent>()->setSpriteResByPath("img_background");
    sprite->component<TransformComponent>();

    //Baskets
    createBasket("blue", {0, 250});
//    createBasket("red", {-200, 250});
//    createBasket("green", {200, 250});

//    //Menu button
//    auto button = manager<EntityManager>()->add(
//        ButtonBuilder(shared_from_this())
//            .idlePath("start_btn_idle")
//            .build()
//    );
//    button->transform()->setPos({0, -260, 0});
//    button->transform()->setScale(0.2f);
}

} // flappy
