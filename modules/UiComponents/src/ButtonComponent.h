#pragma once

#include <glm/vec3.hpp>

#include <EntityManager.h>

namespace flappy {

class ButtonComponent: public Component {
public:
    void update(TimeDelta dt);

    void setOnClick(std::function<void()> onClick) {
        m_onClick = onClick;
    }

private:
    bool isInField();
    bool m_mouseDown = false;
    std::function<void()> m_onClick;
    glm::vec3 m_baseScale;
};

} // flappy
