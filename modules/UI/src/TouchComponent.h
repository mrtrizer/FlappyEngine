#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Handle.hpp>
#include <EnableSelfHandle.hpp>
#include <EventBus.h>

namespace flappy
{
class Entity;
class Box2DBodyComponent;
class UIManager;
class Hierarchy;

class REFLECT TouchComponent : public EnableSelfHandle<TouchComponent>
{
public:
    struct TouchEvent: public IEvent {
        TouchEvent(glm::vec2 pos, int index)
            : pos(pos), index(index)
        {}
        glm::vec2 pos;
        int index;
    };

    struct TouchDownEvent: public TouchEvent {
        using TouchEvent::TouchEvent;
    };

    struct TouchUpEvent: public TouchEvent {
        using TouchEvent::TouchEvent;
    };

    struct TouchMoveEvent: public TouchEvent {
        using TouchEvent::TouchEvent;
    };

    TouchComponent(Handle<Entity> entity);
    ~TouchComponent();

    void touchDown(glm::vec2 pos, int index);
    void touchUp(glm::vec2 pos, int index);
    void touchMove(glm::vec2 pos, int index);

    bool testPoint(glm::vec2 pos);
    EventBus& eventBus() { return m_eventBus; }
    
private:
    Handle<UIManager> m_uiManager;
    Handle<Box2DBodyComponent> m_box2dBodyComponent;
    EventBus m_eventBus;
};

} // flappy
