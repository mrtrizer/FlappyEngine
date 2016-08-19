#pragma once

#include <core/component.h>

namespace game {

class BasketCtrl: public flappy::Component {
public:
    void update(flappy::TimeDelta);
    std::string color;
};

} // game
