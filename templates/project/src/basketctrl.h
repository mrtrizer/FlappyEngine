#pragma once

#include <core/component.h>

namespace game {

using namespace flappy;

class BasketCtrl: public Component {
public:
    void update(TimeDelta);
    string color;
};

} // game
