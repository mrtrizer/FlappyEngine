#include "InputManager.h"

namespace flappy
{

using namespace glm;

bool InputManager::mouseDown() const
{
    return m_mouseDown == 1;
}

bool InputManager::mouseUp() const
{
    return m_mouseUp == 1;
}

bool InputManager::mousePressed() const
{
    return m_mousePressed;
}

vec2 InputManager::mousePos() const
{
    return m_mousePos;
}

void InputManager::update(DeltaTime)
{
    if (--m_mouseDown < 0)
        m_mouseDown = 0;
    if (--m_mouseUp < 0)
        m_mouseUp = 0;
}

void InputManager::setMouseDown(const vec2 &mousePos)
{
    m_mouseDown = 2;
    m_mousePressed = true;
    this->m_mousePos = mousePos;

    InputManager::MouseDownEvent mouseDownEvent;
    mouseDownEvent.pos = mousePos;
    mouseDownEvent.button = InputManager::MouseButton::LEFT;
    events()->post(mouseDownEvent);
}

void InputManager::setMouseUp(const vec2 &mousePos)
{
    m_mouseUp = 2;
    m_mousePressed = false;
    this->m_mousePos = mousePos;

    InputManager::MouseDownEvent mouseUpEvent;
    mouseUpEvent.pos = mousePos;
    mouseUpEvent.button = InputManager::MouseButton::LEFT;
    events()->post(mouseUpEvent);
}

void InputManager::setMousePos(const vec2 &mousePos)
{
    this->m_mousePos = mousePos;

    InputManager::MouseMoveEvent mouseMoveEvent;
    mouseMoveEvent.pos = mousePos;
    events()->post(mouseMoveEvent);
}

} // flappy
