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

    static bool isMouseDown() {return getInst()->isMouseDown_();}
    static bool isMouseUp() {return getInst()->isMouseUp_();}
    static bool isMousePressed() {return getInst()->isMousePressed_();}
    static glm::vec3 getMousePos() {return getInst()->getMousePos_();}

    void update(TimeDelta);
    void setMouseDown();
    void setMouseUp();
    void mouseMove(glm::vec3 mousePos);
private:
    bool isMouseDown_();
    bool isMouseUp_();
    bool isMousePressed_();
    glm::vec3 getMousePos_();

    glm::vec3 mousePos;
    int mouseDown = 0;
    int mouseUp = 0;
    bool mousePressed = false;
};

#endif // INPUTMANAGER_H
