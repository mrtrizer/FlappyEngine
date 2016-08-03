#pragma once

#include <core/component.h>

namespace game {

using namespace flappy;

class BallCtrl: public Component {
public:
    void update(TimeDelta dt);

    float m_speed = 15;
    float m_slideSpeed = 0;
    string color;
};

} // game
