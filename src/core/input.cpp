#include "input.h"

Input::Input()
{

}
bool Input::isMouseDown() {
    return mouseDown == 1;
}

bool Input::isMouseUp() {
    return mouseUp == 1;
}

bool Input::isMousePressed() {
    return mousePressed;
}

glm::vec3 Input::getMousePos() {
    return mousePos;
}

void Input::update(TimeDelta) {
    if (--mouseDown < 0)
        mouseDown = 0;
    if (--mouseUp < 0)
        mouseUp = 0;
}


void Input::setMouseDown() {
    mouseDown = 2;
    mousePressed = true;
}

void Input::setMouseUp() {
    mouseUp = 2;
    mousePressed = false;
}

void Input::mouseMove(glm::vec3 mousePos) {
    this->mousePos = mousePos;
}
