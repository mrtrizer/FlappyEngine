#include "TouchInputManager.h"

#include <Hierarchy.hpp>
#include <EventBus.h>
#include <Handle.hpp>

namespace flappy
{

using namespace glm;

TouchInputManager::TouchInputManager(Handle<Hierarchy> hierarchy)
{}
    
void TouchInputManager::update(float dt) {
    if (--m_touchDown < 0)
        m_touchDown = 0;
    if (--m_touchUp < 0)
        m_touchUp = 0;
}

bool TouchInputManager::isTouchDown() const
{
    return m_touchDown == 1;
}

bool TouchInputManager::isTouchUp() const
{
    return m_touchUp == 1;
}

bool TouchInputManager::isTouchPressed() const
{
    return m_touchPressed;
}

vec2 TouchInputManager::touchPos() const
{
    return m_touchPos;
}

void TouchInputManager::setTouchDown(const vec2 &touchPos, int index)
{
    m_touchDown = 2;
    m_touchPressed = true;
    this->m_touchPos = touchPos;

    TouchDownEvent touchDownEvent;
    touchDownEvent.pos = touchPos;
    touchDownEvent.index = index;
    m_eventBus.post(touchDownEvent);
}

void TouchInputManager::setTouchUp(const vec2 &touchPos, int index)
{
    m_touchUp = 2;
    m_touchPressed = false;
    this->m_touchPos = touchPos;

    TouchUpEvent touchUpEvent;
    touchUpEvent.pos = touchPos;
    touchUpEvent.index = index;
    m_eventBus.post(touchUpEvent);
}

void TouchInputManager::setTouchPos(const vec2 &touchPos, int index)
{
    this->m_touchPos = touchPos;

    TouchMoveEvent touchMoveEvent;
    touchMoveEvent.pos = touchPos;
    touchMoveEvent.index = index;
    m_eventBus.post(touchMoveEvent);
}

} // flappy
