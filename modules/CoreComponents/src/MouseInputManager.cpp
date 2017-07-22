#include "MouseInputManager.h"

namespace flappy
{

using namespace glm;

bool MouseInputManager::isMouseDown(MouseButton mouseButton) const
{
    (void)mouseButton;
    return m_mouseDown == 1;
}

bool MouseInputManager::isMouseUp(MouseButton mouseButton) const
{
    (void)mouseButton;
    return m_mouseUp == 1;
}

bool MouseInputManager::isMousePressed(MouseButton mouseButton) const
{
    (void)mouseButton;
    return m_mousePressed;
}

vec2 MouseInputManager::mousePos() const
{
    return m_mousePos;
}

void MouseInputManager::update(DeltaTime)
{
    if (--m_mouseDown < 0)
        m_mouseDown = 0;
    if (--m_mouseUp < 0)
        m_mouseUp = 0;
}

void MouseInputManager::setMouseDown(const vec2 &mousePos)
{
    m_mouseDown = 2;
    m_mousePressed = true;
    this->m_mousePos = mousePos;

    MouseInputManager::MouseDownEvent mouseDownEvent;
    mouseDownEvent.pos = mousePos;
    mouseDownEvent.button = MouseInputManager::MouseButton::LEFT;
    events()->post(mouseDownEvent);
}

void MouseInputManager::setMouseUp(const vec2 &mousePos)
{
    m_mouseUp = 2;
    m_mousePressed = false;
    this->m_mousePos = mousePos;

    MouseInputManager::MouseDownEvent mouseUpEvent;
    mouseUpEvent.pos = mousePos;
    mouseUpEvent.button = MouseInputManager::MouseButton::LEFT;
    events()->post(mouseUpEvent);
}

void MouseInputManager::setMousePos(const vec2 &mousePos)
{
    this->m_mousePos = mousePos;

    MouseInputManager::MouseMoveEvent mouseMoveEvent;
    mouseMoveEvent.pos = mousePos;
    events()->post(mouseMoveEvent);
}

} // flappy
