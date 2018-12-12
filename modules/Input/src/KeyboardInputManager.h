#pragma once

#include <glm/vec2.hpp>

#include "KeyCode.h"

#include <IEvent.h>
#include <Updatable.hpp>
#include <Handle.hpp>

namespace flappy
{
    class EventBus;
    
    class [[mgr, require_mgr("EventBus")]] KeyboardInputManager : public Updatable<KeyboardInputManager>
    {
    public:
        struct KeyDownEvent: public IEvent {
            KeyCode keyCode;
        };

        struct KeyUpEvent: public IEvent {
            KeyCode keyCode;
        };

        KeyboardInputManager(Handle<Hierarchy> hierarchy);

        void update(float dt);
        
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

        KeyStatus m_keyStatus[(unsigned)KeyCode::__ENUM_SIZE__];
        Handle<EventBus> m_eventBus;
    };

} // flappy
