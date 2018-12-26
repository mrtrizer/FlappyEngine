#pragma once

#include <glm/vec2.hpp>

#include "KeyCode.h"

#include <IEvent.h>
#include <Updatable.hpp>
#include <Handle.hpp>
#include <EventBus.h>

namespace flappy
{
    class EventBus;
    
    class [[manager]] KeyboardInputManager : public Updatable<KeyboardInputManager>
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
        EventBus& eventBus() { return m_eventBus; }

    private:
        struct KeyStatus {
            unsigned downEventCounter = 0;
            unsigned upEventCounter = 0;
            bool pressedFlag = false;
        };

        std::vector<KeyStatus> m_keyStatus {(unsigned)KeyCode::__ENUM_SIZE__};
        EventBus m_eventBus;
    };

} // flappy
