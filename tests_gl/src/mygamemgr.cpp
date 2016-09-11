#include "mygamemgr.h"
#include <core/texture.h>
#include <core/atlas.h>
#include <core/resourcemgr.h>
#include <core/sprite.h>

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void MyScene::createBasket(string color, vec2 pos) {
    EM->create([=](EP e) {
        auto sprite = e->add<Sprite>();
        sprite->setPath(string("atlas_baskets:") + color);

        auto transform = e->add<Transform>();
        transform->setPos(vec3(pos, 0));
        transform->setScale(20);

        e->add<BasketCtrl>()->setColor(color);
    });
}

void MyScene::init() {
    Atlas atlas("img_baskets");
    atlas.addRect("blue",{0,0,0.333f,1});
    atlas.addRect("green",{0.333f,0,0.333f * 2.0f,1.0f});
    atlas.addRect("red",{0.333f * 2.0f,0,0.333 * 3.0f,1.0f});
    RES_MGR->set("atlas_baskets", atlas);

    //Camera
    EM->create([=](EP e){
        e->add<Camera>();
    });

    //Game controller
    EM  ->create([=](EP e){
        e->add<GameCtrl>();
    });

    //Background
    EM->create([=](EP e){
        auto sprite = e->add<Sprite>();
        sprite->setPath("img_background");
        auto transform = e->add<Transform>();
        transform->setScale(200);
    });

    //Baskets
    createBasket("blue", {0, 40});
    createBasket("red", {-30, 40});
    createBasket("green", {30, 40});
}

} // flappy
