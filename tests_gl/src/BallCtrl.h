#pragma once

#include <EntityManager.h>

namespace game {

class BallCtrl: public flappy::Component {
public:
    BallCtrl(){
        LOGI("I've birth.");
    }

    void update(flappy::TimeDelta dt);

    float speed = 15;
    float slideSpeed = 0;
    std::string color;
};

} // game
