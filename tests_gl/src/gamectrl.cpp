#include "gamectrl.h"

#include <glm/gtc/random.hpp>
#include <core/sprite.h>
#include <core/inputmanager.h>
#include <core/presenter.h>

#include "ballctrl.h"

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameCtrl::createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    string color = colors[linearRand(0, 2)];
    EM->create([=](EP e) {
        switch (linearRand(0,2)) {
        case 0: {
            auto sprite = e->create<Sprite>();
            sprite->setPath(string("atlas_baskets:") + color);
            sprite->setColor({1,1,1,0.5f});
            break;
        }
        case 1:
            e->create<CircleShape>()->setColor({0,0,0});
            break;
        case 2:
            e->create<RectShape>()->setColor({0,1,0,0.5f});
            break;
        }
        float randX = linearRand(-30, 30);
        auto transform = e->create<Transform>();
        transform->setPos({randX,-50, 0});
        transform->setScale(10);
        e->create<BallCtrl>()->color = color;
    });
}


void GameCtrl::update(TimeDelta dt) {
    m_time += dt;
    if (m_time > spawnTime) {
        m_time = 0;
        createBall();
    }
    if (INPUT->mouseDown())
        m_mouseDownPos = INPUT->mousePos();
    if (INPUT->mouseUp())
        EM->each<BallCtrl>([this](EP e) {
            auto ballCtrl = e->get<BallCtrl>();
            if (m_mouseDownPos.x - INPUT->mousePos().x > 0)
                ballCtrl->slideSpeed = glm::max(-30.0f, ballCtrl->slideSpeed - 10);
            else
                ballCtrl->slideSpeed = glm::min(30.0f, ballCtrl->slideSpeed + 10);
        });
}


} // game
