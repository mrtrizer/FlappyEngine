#pragma once

#include <SDL2/SDL.h>
#include <Handle.hpp>
#include <ISubscription.h>

namespace flappy {

class Hierarchy;
class Sdl2Manager;
class KeyboardInputManager;
    
class REFLECT Sdl2KeyboardInput
{
public:
    Sdl2KeyboardInput(Handle<Hierarchy> hierarchy);
    
private:
    Handle<Sdl2Manager> m_sdl2Manager;
    Handle<KeyboardInputManager> m_keyboardInputManager;
    std::shared_ptr<ISubscription> m_subscription;
};

} // flappy
