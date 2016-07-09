#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <glm/vec3.hpp>
#include <core/tools.h>
#include <core/entitymanager.h>

#include "manager.h"

class Input: public Manager<Input>
{
public:
    Input();

    bool isMouseDown();
    bool isMouseUp();
    bool isMousePressed();
    glm::vec3 getMousePos();

    void update(TimeDelta);
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
