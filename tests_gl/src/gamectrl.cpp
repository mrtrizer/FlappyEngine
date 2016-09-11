#include "gamectrl.h"

#include <glm/gtc/random.hpp>
#include <core/sprite.h>
#include <core/inputmgr.h>
#include <core/circleshape.h>

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
        case 0:
            e->add<Sprite>()->setPath(string("atlas_baskets:") + color);
            break;
        case 1:
            e->add<CircleShape>()->setColor({0,0,0});
            break;
        case 2:
            e->add<RectShape>()->setColor({0,0.5f,0,0.5f});
            break;
        }
        float randX = linearRand(-30, 30);
        auto transform = e->add<Transform>();
        transform->setPos({randX,-50, 0});
        transform->setScale(10);
        e->add<BallCtrl>()->color = color;
    });
}


void GameCtrl::update(TimeDelta dt) {
    m_time += dt;
    if (m_time > spawnTime) {
        m_time = 0;
        createBall();
    }
    if (INPUT->isMouseDown())
        m_mouseDownPos = INPUT->getMousePos();
    if (INPUT->isMouseUp())
        EM->each<BallCtrl>([this](EP e) {
            auto ballCtrl = e->get<BallCtrl>();
            if (m_mouseDownPos.x - INPUT->getMousePos().x > 0)
                ballCtrl->slideSpeed = glm::max(-30.0f, ballCtrl->slideSpeed - 10);
            else
                ballCtrl->slideSpeed = glm::min(30.0f, ballCtrl->slideSpeed + 10);
        });
}


} // game
