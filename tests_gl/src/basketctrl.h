#pragma once

#include <core/component.h>

namespace game {

class BasketCtrl: public flappy::Component {
public:
    void update(flappy::TimeDelta);
    void setColor(std::string color) {m_color = color;}
    std::string color() const { return m_color; }

private:
    std::string m_color;
};

} // game
