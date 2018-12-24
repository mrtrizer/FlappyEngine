#pragma once

#include <glm/vec2.hpp>

#include <IEvent.h>
#include <Handle.hpp>
#include <EventBus.h>

namespace flappy
{
class EventBus;
class Hierarchy;

class [[manager]] TouchInputManager
{
public:

    struct TouchDownEvent: public IEvent {
        glm::vec2 pos;
        int index;
    };

    struct TouchUpEvent: public IEvent {
        glm::vec2 pos;
        int index;
    };

    struct TouchMoveEvent: public IEvent {
        glm::vec2 pos;
        int index;
    };

    TouchInputManager(Handle<Hierarchy>);
    
    void update(float dt);

    bool isTouchDown() const;
    bool isTouchUp() const;
    bool isTouchPressed() const;
    glm::vec2 touchPos() const;

    void setTouchDown(const glm::vec2 &touchPos, int index);
    void setTouchUp(const glm::vec2 &touchPos, int index);
    void setTouchPos(const glm::vec2 &touchPos, int index);
    
    EventBus& eventBus() { return m_eventBus; }
    
private:
    EventBus m_eventBus;
    glm::vec2 m_touchPos;
    int m_touchDown = 0;
    int m_touchUp = 0;
    bool m_touchPressed = false;
};

} // flappy
