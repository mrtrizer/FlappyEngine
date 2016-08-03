#include "mygamemgr.h"

namespace game {

void createBasket(string color, vec2 pos) {
    EM::create([=](EP e){
        e->add<Sprite>(string("orb_") + color,20, 20);
        e->add<Transform>()->setPos(vec3(pos, 0));
        e->add<BasketCtrl>()->color = color;
    });
}

void MyGameMgr::init() {
    //Camera
    EM::create([=](EP e){
        e->add<Camera>();
    });

    //Game controller
    EM::create([=](EP e){
        e->add<GameCtrl>();
    });

    //Background
    EM::create([=](EP e){
        e->add<Sprite>("background",200, 200, 1);
        e->add<Transform>();
    });

    //Baskets
    createBasket("blue", vec2(0, 40));
    createBasket("red", vec2(-30, 40));
    createBasket("green", vec2(30, 40));
}

} // flappy
