#pragma once

#include <core/component.h>
#include <core/transform.h>

namespace game {

class GameCtrl: public flappy::Component {
public:
    void update(flappy::TimeDelta dt);

    float spawnTime = 2.0f;

private:
    float m_time = 0;
    glm::vec2 m_mouseDownPos;
    std::string m_color;

    void createBall();
};

} // game
