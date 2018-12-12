#include "MouseInputManager.h"

#include <EventBus.h>

namespace flappy
{

using namespace glm;

MouseInputManager::MouseInputManager(Handle<Hierarchy> hierarchy)
    : Updatable<MouseInputManager>(hierarchy)
    , m_eventBus(hierarchy->manager<EventBus>())
{}
    
void MouseInputManager::update(float dt) {
    if (--m_mouseDown < 0)
        m_mouseDown = 0;
    if (--m_mouseUp < 0)
        m_mouseUp = 0;
}

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

void MouseInputManager::setMouseDown(MouseButton mouseButton, const vec2 &mousePos)
{
    (void)mouseButton;

    m_mouseDown = 2;
    m_mousePressed = true;
    this->m_mousePos = mousePos;

    MouseInputManager::MouseDownEvent mouseDownEvent;
    mouseDownEvent.pos = mousePos;
    mouseDownEvent.button = MouseInputManager::MouseButton::LEFT;
    m_eventBus->post(mouseDownEvent);
}

void MouseInputManager::setMouseUp(MouseButton mouseButton, const vec2 &mousePos)
{
    (void)mouseButton;

    m_mouseUp = 2;
    m_mousePressed = false;
    this->m_mousePos = mousePos;

    MouseInputManager::MouseUpEvent mouseUpEvent;
    mouseUpEvent.pos = mousePos;
    mouseUpEvent.button = MouseInputManager::MouseButton::LEFT;
    m_eventBus->post(mouseUpEvent);
}

void MouseInputManager::setMousePos(MouseButton mouseButton, const vec2 &mousePos)
{
    (void)mouseButton;

    this->m_mousePos = mousePos;

    MouseInputManager::MouseMoveEvent mouseMoveEvent;
    mouseMoveEvent.pos = mousePos;
    m_eventBus->post(mouseMoveEvent);
}

} // flappy
