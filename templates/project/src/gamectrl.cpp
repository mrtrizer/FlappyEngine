#include "gamectrl.h"

#include <glm/gtc/random.hpp>
#include <core/sprite.h>
#include <core/inputmgr.h>

#include "ballctrl.h"

namespace game {

using namespace glm;

void createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    string color = colors[linearRand(0, 2)];
    EM::create([=](EP e) {
        e->add<Sprite>(string("orb_") + color,10, 10);
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
    if (Input::isMouseDown())
        m_mouseDownPos = Input::getMousePos();
    if (Input::isMouseUp())
        EM::each<BallCtrl>([this](EP e) {
            auto ballCtrl = e->get<BallCtrl>();
            if (m_mouseDownPos.x - Input::getMousePos().x > 0)
                ballCtrl->m_slideSpeed = glm::max(-30.0f, ballCtrl->m_slideSpeed - 10);
            else
                ballCtrl->m_slideSpeed = glm::min(30.0f, ballCtrl->m_slideSpeed + 10);
        });
}


} // game
