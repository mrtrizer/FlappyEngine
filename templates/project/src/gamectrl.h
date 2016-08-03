#pragma once

#include <core/component.h>
#include <core/transform.h>

namespace game {

using namespace flappy;
using namespace glm;

class GameCtrl: public Component {
public:
    void update(TimeDelta dt);

    float spawnTime = 2.0f;

private:
    float m_time = 0;
    vec3 m_mouseDownPos;
    string m_color;
};

} // game
