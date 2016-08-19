#include "gamectrl.h"

#include <glm/gtc/random.hpp>
#include <core/sprite.h>
#include <core/inputmgr.h>

#include "ballctrl.h"

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameCtrl::createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    string color = colors[linearRand(0, 2)];
    EM->create([=](EP e) {
        e->add<Sprite>(string("atlas_baskets:") + color,10, 10);
        float randX = linearRand(-30, 30);
        e->add<Transform>()->setPos(vec3(randX,-50, 0));
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
