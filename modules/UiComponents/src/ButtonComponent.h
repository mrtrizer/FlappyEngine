#pragma once

#include <glm/vec3.hpp>

#include <SceneManager.h>

namespace flappy {

class ButtonComponent: public Component {
public:
    struct OnButtonPress: public IEvent {};
    struct OnButtonRelease: public IEvent {};
    struct OnButtonClick: public IEvent {};

    void update(DeltaTime dt);

private:
    bool isInField();
    bool m_mouseDown = false;
};

} // flappy
