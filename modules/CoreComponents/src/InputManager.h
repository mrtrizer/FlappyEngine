#pragma once

#include <glm/vec3.hpp>

#include <Manager.h>
#include <Tools.h>

namespace flappy
{

class InputManager: public Manager<InputManager>
{
public:
    enum MouseButton {
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
        float x;
        float y;
        MouseButton button;
    };

    struct MouseUpEvent: public IEvent {
        float x;
        float y;
        MouseButton button;
    };

    struct MouseMoveEvent: public IEvent {
        float x;
        float y;
    };

    bool mouseDown() const;
    bool mouseUp() const;
    bool mousePressed() const;
    glm::vec2 mousePos() const;

    void update(DeltaTime) override;
    void setMouseDown();
    void setMouseUp();
    void setMousePos(const glm::vec2& m_mousePos);
private:

    glm::vec2 m_mousePos;
    int m_mouseDown = 0;
    int m_mouseUp = 0;
    bool m_mousePressed = false;
};

} // flappy
