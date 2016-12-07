#include "GameCtrl.h"

#include <glm/gtc/random.hpp>

#include <InputManager.h>
#include <ResManager.h>

#include <SpriteComponent.h>
#include <PresenterComponent.h>
#include <QuadRes.h>

#include "BallCtrl.h"

namespace game {

using namespace std;
using namespace glm;
using namespace flappy;

void GameCtrl::createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    string color = colors[linearRand(0, 2)];
    EM->create([=](EP e) {
        float randX = linearRand(-30, 30);
        auto transform = e->create<TransformComponent>();
        transform->setPos({randX,-50, 0});

        transform->setScale(10);

        switch (linearRand(0,2)) {
        case 0: {
            auto sprite = e->create<SpriteComponent>();
            sprite->setQuad(MGR<ResManager>()->getRes<QuadRes>("img_baskets:" + color));
            sprite->setColor({1,1,1,0.5f});
            transform->setScale(0.5f);
            break;
        }
        case 1:
            e->create<CircleShape>()->setColor({0,0,0});
            break;
        case 2:
            e->create<RectShape>()->setColor({0,1,0,0.5f});
            break;
        }



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