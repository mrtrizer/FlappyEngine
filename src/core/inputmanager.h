#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec3.hpp>
#include <entityx/entityx.h>
#include <core/gtools.h>

#include "manager.h"

class InputManager: public Manager<InputManager>
{
public:
    InputManager();

    bool isMouseDown();
    bool isMouseUp();
    bool isMousePressed();
    glm::vec3 getMousePos();

    void update(entityx::TimeDelta);
    void setMouseDown();
    void setMouseUp();
    void mouseMove(glm::vec3 mousePos);
private:
    glm::vec3 mousePos;
    int mouseDown = 0;
    int mouseUp = 0;
    bool mousePressed = false;
};

#endif // INPUTMANAGER_H