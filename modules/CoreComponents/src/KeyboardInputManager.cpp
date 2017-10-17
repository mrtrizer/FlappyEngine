#include "KeyboardInputManager.h"

#include <Entity.h>

namespace flappy
{

KeyboardInputManager::KeyboardInputManager() {

    subscribe([this](UpdateEvent) {
        for (int i = 0; i < (int)KeyCode::__ENUM_SIZE__; i++) {
            if (m_keyStatus[i].downEventCounter > 0)
                m_keyStatus[i].downEventCounter--;
            if (m_keyStatus[i].upEventCounter > 0)
                m_keyStatus[i].upEventCounter--;
        }
    });

}

void KeyboardInputManager::setKeyDown(KeyCode keyCode)
{
    m_keyStatus[(int)keyCode].downEventCounter = 2;
    m_keyStatus[(int)keyCode].pressedFlag = true;
    KeyDownEvent keyDownEvent;
    keyDownEvent.keyCode = keyCode;
    entity()->events()->post(keyDownEvent);
}

void KeyboardInputManager::setKeyUp(KeyCode keyCode)
{
    m_keyStatus[(int)keyCode].upEventCounter = 2;
    m_keyStatus[(int)keyCode].pressedFlag = false;
    KeyUpEvent keyUpEvent;
    keyUpEvent.keyCode = keyCode;
    entity()->events()->post(keyUpEvent);
}

bool KeyboardInputManager::isKeyDown(KeyCode keyCode)
{
    return m_keyStatus[(int)keyCode].downEventCounter == 1;
}

bool KeyboardInputManager::isKeyUp(KeyCode keyCode)
{
    return m_keyStatus[(int)keyCode].upEventCounter == 1;
}

bool KeyboardInputManager::isKeyPressed(KeyCode keyCode)
{
    return m_keyStatus[(int)keyCode].pressedFlag;
}

} // flappy
