#pragma once

#include <glm/vec2.hpp>

#include <IEvent.h>
#include <Updatable.hpp>
#include <EventBus.h>

namespace flappy
{

class REFLECT MouseInputManager : public Updatable<MouseInputManager>
{
public:
    enum class MouseButton {
        LEFT,
        RIGHT,
        CENTER,
        BUTTON_1,
        BUTTON_2,
        BUTTON_3,
        BUTTON_4,
        BUTTON_5,
        BUTTON_6,
        BUTTON_7
    };

    struct MouseDownEvent: public IEvent {
        glm::vec2 pos;
        MouseButton button;
    };

    struct MouseUpEvent: public IEvent {
        glm::vec2 pos;
        MouseButton button;
    };

    struct MouseMoveEvent: public IEvent {
        glm::vec2 pos;
    };

    MouseInputManager(Handle<Hierarchy>);
    
    void update(float dt);

    bool isMouseDown(MouseButton mouseButton = MouseButton::LEFT) const;
    bool isMouseUp(MouseButton mouseButton = MouseButton::LEFT) const;
    bool isMousePressed(MouseButton mouseButton = MouseButton::LEFT) const;
    const glm::vec2& mousePos() const { return m_mousePos; }

    void setMouseDown(MouseButton mouseButton, const glm::vec2 &mousePos);
    void setMouseUp(MouseButton mouseButton, const glm::vec2 &mousePos);
    void setMousePos(MouseButton mouseButton, const glm::vec2& m_mousePos);
    
    EventBus& eventBus() { return m_eventBus; }
private:
    EventBus m_eventBus;
    glm::vec2 m_mousePos;
    int m_mouseDown = 0;
    int m_mouseUp = 0;
    bool m_mousePressed = false;
};

} // flappy
