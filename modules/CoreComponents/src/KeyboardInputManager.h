#pragma once

#include <glm/vec2.hpp>

#include <Manager.h>
#include <Tools.h>

#include "KeyCode.h"

namespace flappy
{

class KeyboardInputManager: public Manager<KeyboardInputManager>
{
public:
    struct KeyDown: public IEvent {
        KeyCode keyCode;
    };

    struct KeyUp: public IEvent {
        KeyCode keyCode;
    };

    void setKeyDown(KeyCode keyCode);
    void setKeyUp(KeyCode keyCode);

    bool isKeyDown(KeyCode keyCode);
    bool isKeyUp(KeyCode keyCode);
    bool isKeyPressed(KeyCode keyCode);

private:
    struct KeyStatus {
        unsigned downEventCounter = 0;
        unsigned upEventCounter = 0;
        bool pressedFlag = false;
    };

    void update(DeltaTime dt) final;

    KeyStatus m_keyStatus[(unsigned)KeyCode::__ENUM_SIZE__];
};

} // flappy
