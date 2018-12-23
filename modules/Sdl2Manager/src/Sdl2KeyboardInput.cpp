#include "Sdl2KeyboardInput.h"

#include <Hierarchy.hpp>
#include <KeyboardInputManager.h>
#include <EventBus.h>

#include "Sdl2Manager.h"

namespace flappy {

Sdl2KeyboardInput::Sdl2KeyboardInput(Handle<Hierarchy> hierarchy)
    : m_sdl2Manager(hierarchy->manager<Sdl2Manager>())
    , m_keyboardInputManager(hierarchy->manager<KeyboardInputManager>())
{
    m_subscription = m_sdl2Manager->eventBus().subscribe([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_KEYDOWN) {
            m_keyboardInputManager->setKeyDown((KeyCode)e.event.key.keysym.scancode);
        }
        if (e.event.type == SDL_KEYUP) {
            m_keyboardInputManager->setKeyUp((KeyCode)e.event.key.keysym.scancode);
        }
    });
}

} // flappy
