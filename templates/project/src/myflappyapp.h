#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <functional>
#include <glm/gtc/random.hpp>

#include <core/scenemgr.h>
#include <core/transform.h>
#include <core/animation.h>
#include <core/flappyapp.h>
#include <core/entitymgr.h>
#include <core/inputmgr.h>
#include <ui/button.h>

class BallCtrl: public Component {
public:
    void update(TimeDelta dt) {
        m_slideSpeed += dt * (m_slideSpeed > 0?-5:5);
        entity()->transform()->move(glm::vec3(0, m_speed * dt, 0));
        entity()->transform()->move(glm::vec3(m_slideSpeed * dt, 0, 0));
        if (entity()->transform()->pos().y > 40)
            EM::remove(entity());
    }

    float m_speed = 15;
    float m_slideSpeed = 0;
    std::string color;
};

class BasketCtrl: public Component {
public:
    void update(TimeDelta) {
        EM::each<BallCtrl>([this](EP e) {
            float r = entity()->get<Sprite>()->width() * 0.5f * entity()->transform()->scale().x;
            float minDist = e->get<Sprite>()->width() * 0.5f + r;
            if (glm::distance(e->transform()->pos(), entity()->transform()->pos()) < minDist) {
                EM::remove(e);
                if (e->get<BallCtrl>()->color == color)
                    entity()->transform()->stretch(-0.1f);
            }
        });
    }
    std::string color;
};

void createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    std::string color = colors[glm::linearRand(0, 2)];
    EM::create([=](EP e) {
        e->add<Sprite>(std::string("orb_") + color,10, 10);
        float randX = glm::linearRand(-30, 30);
        e->add<Transform>()->setPos(glm::vec3(randX,-50, 0));
        e->add<BallCtrl>()->color = color;
    });
}

void createBasket(std::string color, glm::vec2 pos) {
    EM::create([=](EP e){
        e->add<Sprite>(std::string("orb_") + color,20, 20);
        e->add<Transform>()->setPos(glm::vec3(pos, 0));
        e->add<BasketCtrl>()->color = color;
    });
}

class GameCtrl: public Component {
public:
    void update(TimeDelta dt) {
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

    float spawnTime = 2.0f;

private:
    float m_time = 0;
    glm::vec3 m_mouseDownPos;
    std::string m_color;
};

class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
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
        createBasket("blue", glm::vec2(0, 40));
        createBasket("red", glm::vec2(-30, 40));
        createBasket("green", glm::vec2(30, 40));
    }
};

#endif // MYFLAPPYAPP_H
