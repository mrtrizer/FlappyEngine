#ifndef MYFLAPPYAPP_H
#define MYFLAPPYAPP_H

#include <functional>
#include <glm/gtc/random.hpp>

#include <core/scene.h>
#include <core/transform.h>
#include <core/animation.h>
#include <core/flappyapp.h>
#include <core/entitymanager.h>
#include <core/input.h>
#include <ui/button.h>

class BallCtrl: public BaseComponent<BallCtrl> {
public:
    void update(TimeDelta dt) {
        slideSpeed += dt * (slideSpeed > 0?-5:5);
        entity()->transform()->pos.y += speed * dt;
        entity()->transform()->pos.x += slideSpeed * dt;
        if (entity()->transform()->pos.y > 40)
            REMOVE(entity());
    }

    float speed = 15;
    float slideSpeed = 0;
    std::string color;
};

class BasketCtrl: public BaseComponent<BasketCtrl> {
public:
    void update(TimeDelta) {
        EACH<BallCtrl>([this](EP e) {
            float r = entity()->get<Sprite>()->getWidth() * 0.5f * entity()->transform()->scale;
            float minDist = e->get<Sprite>()->getWidth() * 0.5f + r;
            if (glm::distance(e->transform()->pos, entity()->transform()->pos) < minDist) {
                REMOVE(e);
                if (e->get<BallCtrl>()->color == color)
                    entity()->transform()->scale -= 0.1f;
            }
        });
    }
    std::string color;
};

void createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    std::string color = colors[glm::linearRand(0, 2)];
    CREATE([=](EP e) {
        e->add<Sprite>(std::string("orb_") + color,10, 10);
        float randX = glm::linearRand(-30, 30);
        e->add<Transform>()->pos = glm::vec3(randX,-50, 0);
        e->add<BallCtrl>()->color = color;
    });
}

void createBasket(std::string color, glm::vec2 pos) {
    CREATE([=](EP e){
        e->add<Sprite>(std::string("orb_") + color,20, 20);
        e->add<Transform>()->pos = glm::vec3(pos, 0);
        e->add<BasketCtrl>()->color = color;
    });
}

class GameCtrl: public BaseComponent<GameCtrl> {
public:
    void update(TimeDelta dt) {
        _time += dt;
        if (_time > spawnTime) {
            _time = 0;
            createBall();
        }
        if (Input::isMouseDown())
            _mouseDownPos = Input::getMousePos();
        if (Input::isMouseUp())
            EACH<BallCtrl>([this](EP e) {
                auto ballCtrl = e->get<BallCtrl>();
                if (_mouseDownPos.x - Input::getMousePos().x > 0)
                    ballCtrl->slideSpeed = glm::max(-30.0f, ballCtrl->slideSpeed - 10);
                else
                    ballCtrl->slideSpeed = glm::min(30.0f, ballCtrl->slideSpeed + 10);
            });
    }

    float spawnTime = 2.0f;

private:
    float _time = 0;
    glm::vec3 _mouseDownPos;
    std::string _color;
};

class MyFlappyApp : public FlappyApp
{
public:
    void init() override {
        //Camera
        CREATE([=](EP e){
            e->add<Camera>();
        });

        //Game controller
        CREATE([=](EP e){
            e->add<GameCtrl>();
        });

        //Background
        CREATE([=](EP e){
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
