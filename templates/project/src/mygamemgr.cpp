#include "mygamemgr.h"
#include <core/texture.h>
#include <core/atlas.h>
#include <core/resourcemgr.h>

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void MyGameMgr::createBasket(string color, vec2 pos) {
    EM->create([=](EP e){
        e->add<Sprite>(string("atlas_baskets:") + color,20, 20);
        e->add<Transform>()->setPos(vec3(pos, 0));
        e->add<BasketCtrl>()->color = color;
    });
}

void MyGameMgr::init() {

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
    EM->create([=](EP e){
        e->add<GameCtrl>();
    });

    //Background
    EM->create([=](EP e){
        e->add<Sprite>("img_background",200, 200);
        e->add<Transform>();
    });

    //Baskets
    createBasket("blue", vec2(0, 40));
    createBasket("red", vec2(-30, 40));
    createBasket("green", vec2(30, 40));
}

} // flappy
