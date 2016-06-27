#include "inputmanager.h"

InputManager::InputManager()
{

}
bool InputManager::isMouseDown() {
    return mouseDown == 1;
}

bool InputManager::isMouseUp() {
    return mouseUp == 1;
}

bool InputManager::isMousePressed() {
    return mousePressed;
}

glm::vec3 InputManager::getMousePos() {
    return mousePos;
}

void InputManager::update(entityx::TimeDelta) {
    if (--mouseDown < 0)
        mouseDown = 0;
    if (--mouseUp < 0)
        mouseUp = 0;
}


void InputManager::setMouseDown() {
    mouseDown = 2;
    mousePressed = true;
}

void InputManager::setMouseUp() {
    mouseUp = 2;
    mousePressed = false;
}

void InputManager::mouseMove(glm::vec3 mousePos) {
    this->mousePos = mousePos;
}
