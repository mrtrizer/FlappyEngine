#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Component.h>

namespace flappy
{

class TouchComponent: public Component<TouchComponent>
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

    TouchComponent();

    void touchDown(glm::vec2 pos, int index);
    void touchUp(glm::vec2 pos, int index);
    void touchMove(glm::vec2 pos, int index);

    bool testPoint(glm::vec2 pos);
};

} // flappy
