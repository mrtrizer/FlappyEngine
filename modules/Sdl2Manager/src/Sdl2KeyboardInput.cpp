#include "Sdl2KeyboardInput.h"

#include <Entity.h>
#include <KeyboardInputManager.h>

#include "Sdl2Manager.h"

namespace flappy {

Sdl2KeyboardInput::Sdl2KeyboardInput() {
    addDependency(Sdl2Manager::id());
    addDependency(KeyboardInputManager::id());

    subscribe([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_KEYDOWN) {
            manager<KeyboardInputManager>()->setKeyDown((KeyCode)e.event.key.keysym.scancode);
        }
        if (e.event.type == SDL_KEYUP) {
            manager<KeyboardInputManager>()->setKeyUp((KeyCode)e.event.key.keysym.scancode);
        }
    });
}

} // flappy
