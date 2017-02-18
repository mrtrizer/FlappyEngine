#pragma once

#include <glm/vec3.hpp>

#include <SceneManager.h>

namespace flappy {

class ButtonComponent: public Component {
public:
    struct OnButtonPress {};
    struct OnButtonRelease {};
    struct OnButtonClick {};

    void update(TimeDelta dt);

private:
    bool isInField();
    bool m_mouseDown = false;
};

} // flappy
