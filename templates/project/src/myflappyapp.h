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

class MoveUp: public BaseComponent<MoveUp> {
public:
    void update(TimeDelta dt) {
        slideSpeed += dt * (slideSpeed > 0?-5:5);
        getEntity()->get<Transform>()->pos.y += speed * dt;
        getEntity()->get<Transform>()->pos.x += slideSpeed * dt;
        if (getEntity()->get<Transform>()->pos.y > 40)
            EntityManager::getInst()->remove(getEntity());
    }

    float speed = 15;
    float slideSpeed = 0;
    std::string color;
};

class BasketCtrl: public BaseComponent<BasketCtrl> {
public:
    void update(TimeDelta) {
        EACH<MoveUp>([this](EP e) {
               if (glm::distance(e->get<Transform>()->pos, getEntity()->get<Transform>()->pos) < 15) {
                   REMOVE(e);
                   if (e->get<MoveUp>()->color == color)
                        getEntity()->get<Transform>()->scale -= 0.1f;
               }
           });
    }
    std::string color;
};

void createBall() {
    const char*  colors [] = {"red", "green", "blue"};
    std::string color = colors[glm::linearRand(0, 2)];
    CREATE([=](EP e){
               e->add<Sprite>(std::string("orb_") + color,10, 10);
               e->add<Transform>()->pos = glm::vec3(glm::linearRand(-30, 30),-50, 0);
               e->add<MoveUp>()->color = color;
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
        time += dt;
        if (time > spawnTime) {
            time = 0;
            createBall();
        }
        if (Input::getInst()->isMouseDown())
            mouseDownPos = Input::getInst()->getMousePos();
        if (Input::getInst()->isMouseUp())
            EntityManager::getInst()->each<MoveUp>( [this](std::shared_ptr<Entity> e) {
                if (mouseDownPos.x - Input::getInst()->getMousePos().x > 0)
                    e->get<MoveUp>()->slideSpeed = glm::max(-30.0f, e->get<MoveUp>()->slideSpeed - 10);
                else
                    e->get<MoveUp>()->slideSpeed = glm::min(30.0f, e->get<MoveUp>()->slideSpeed + 10);
            });
    }

    float spawnTime = 2.0f;

private:
    float time = 0;
    glm::vec3 mouseDownPos;
    std::string color;
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
