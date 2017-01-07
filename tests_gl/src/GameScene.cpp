#include "GameScene.h"
#include <AtlasRes.h>
#include <ResManager.h>
#include <SpriteComponent.h>
#include <PresenterComponent.h>
#include <QuadRes.h>
#include <SizeComponent.h>

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameScene::createBasket(string color, vec2 pos) {
    EM->create([=](EP e) {
        auto quad = MGR<ResManager>()->getRes<QuadRes>("img_baskets:" + color);

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
    MGR<ResManager>()->setRes<AtlasRes>("img_baskets", atlas);

    //CameraComponent
    EM->create([=](EP e){
        e->create<CameraComponent>();
    });

    //Game controller
    EM->create([=](EP e){
        e->create<GameCtrl>();
    });

    //Background
    EM->create([=](EP e){
        auto sprite = e->create<SpriteComponent>();
        sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_background"));
        auto transform = e->create<TransformComponent>();
        transform->setScale(2);
    });

    //Baskets
    createBasket("blue", {0, 40});
    createBasket("red", {-30, 40});
    createBasket("green", {30, 40});
}

} // flappy
