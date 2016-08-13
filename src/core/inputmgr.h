#pragma once

#include <glm/vec3.hpp>

#include "manager.h"
#include "tools.h"

namespace flappy {

class InputMgr: public Manager<InputMgr>
{
public:
    bool isMouseDown() const;
    bool isMouseUp() const;
    bool isMousePressed() const;
    glm::vec3 getMousePos() const;

    void update(TimeDelta) override;
    void setMouseDown();
    void setMouseUp();
    void mouseMove(const glm::vec3& m_mousePos);
private:

    glm::vec3 m_mousePos;
    int m_mouseDown = 0;
    int m_mouseUp = 0;
    bool m_mousePressed = false;
};

#define INPUT MGR<InputMgr>()

} // flappy
