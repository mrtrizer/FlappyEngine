#pragma once

#include <core/component.h>

namespace game {

class BallCtrl: public flappy::Component {
public:
    void update(flappy::TimeDelta dt);

    float speed = 15;
    float slideSpeed = 0;
    std::string color;
};

} // game
