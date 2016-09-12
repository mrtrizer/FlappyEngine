#include "inputmanager.h"

namespace flappy {

using namespace glm;

bool InputManager::mouseDown() const {
    return m_mouseDown == 1;
}

bool InputManager::mouseUp() const {
    return m_mouseUp == 1;
}

bool InputManager::mousePressed() const {
    return m_mousePressed;
}

vec2 InputManager::mousePos() const {
    return m_mousePos;
}

void InputManager::update(TimeDelta) {
    if (--m_mouseDown < 0)
        m_mouseDown = 0;
    if (--m_mouseUp < 0)
        m_mouseUp = 0;
}

void InputManager::setMouseDown() {
    m_mouseDown = 2;
    m_mousePressed = true;
}

void InputManager::setMouseUp() {
    m_mouseUp = 2;
    m_mousePressed = false;
}

void InputManager::setMousePos(const vec2 &mousePos) {
    this->m_mousePos = mousePos;
}

} // flappy
