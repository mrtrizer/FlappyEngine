#pragma once

#include <SDL2/SDL.h>
#include <Handle.hpp>
#include <ISubscription.h>

namespace flappy {

class Hierarchy;
class Sdl2Manager;
class MouseInputManager;
    
class Sdl2MouseInput
{
public:
    Sdl2MouseInput(Handle<Hierarchy> hierarchy);
    
private:
    Handle<Sdl2Manager> m_sdl2Manager;
    Handle<MouseInputManager> m_mouseInputManager;
    std::shared_ptr<ISubscription> m_subscription;
};

} // flappy
