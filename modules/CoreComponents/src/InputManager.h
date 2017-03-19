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
        RIGHT
    };

    struct OnMouseDown: public IEvent {
        float x;
        float y;
        MouseButton button;
    };

    struct OnMouseUp: public IEvent {
        float x;
        float y;
        MouseButton button;
    };

    struct OnMouseMove: public IEvent {
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
