#pragma once

#include <glm/vec3.hpp>

#include "manager.h"
#include "tools.h"

namespace flappy {

using namespace std;
using namespace glm;

class InputMgr: public Manager<InputMgr>
{
public:
    bool isMouseDown() const;
    bool isMouseUp() const;
    bool isMousePressed() const;
    vec3 getMousePos() const;

    void update(TimeDelta) override;
    void setMouseDown();
    void setMouseUp();
    void mouseMove(const vec3& m_mousePos);
private:

    vec3 m_mousePos;
    int m_mouseDown = 0;
    int m_mouseUp = 0;
    bool m_mousePressed = false;
};

} // flappy
