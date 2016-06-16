#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec3.hpp>
#include <entityx/entityx.h>

#include "manager.h"

class InputManager: public Manager<InputManager>
{
public:
    InputManager();

    bool isMouseDown() {
        mousePressed = true;
        return mouseDown;
    }

    bool isMouseUp() {
        mousePressed = false;
        return mouseUp;
    }

    bool isMousePressed() {
        return mousePressed;
    }

    glm::vec3 getMousePos() {
        return mousePos;
    }

    void update(entityx::TimeDelta) {
        if (mouseDown)
            mouseDown = false;
        if (mouseUp)
            mouseUp = false;
    }


    void setMouseDown() {
        mouseDown = true;
        mousePressed = true;
    }

    void setMouseUp() {
        mouseUp = true;
        mousePressed = false;
    }

    void mouseMove(glm::vec3 mousePos) {
        this->mousePos = mousePos;
    }

private:
    glm::vec3 mousePos;
    bool mouseDown;
    bool mouseUp;
    bool mousePressed;
};

#endif // INPUTMANAGER_H
