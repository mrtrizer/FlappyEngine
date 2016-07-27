#include "inputmgr.h"

bool InputMgr::isMouseDown() const {
    return m_mouseDown == 1;
}

bool InputMgr::isMouseUp() const {
    return m_mouseUp == 1;
}

bool InputMgr::isMousePressed() const {
    return m_mousePressed;
}

glm::vec3 InputMgr::getMousePos() const {
    return m_mousePos;
}

void InputMgr::update(TimeDelta) {
    if (--m_mouseDown < 0)
        m_mouseDown = 0;
    if (--m_mouseUp < 0)
        m_mouseUp = 0;
}

void InputMgr::setMouseDown() {
    m_mouseDown = 2;
    m_mousePressed = true;
}

void InputMgr::setMouseUp() {
    m_mouseUp = 2;
    m_mousePressed = false;
}

void InputMgr::mouseMove(const glm::vec3 &mousePos) {
    this->m_mousePos = mousePos;
}
