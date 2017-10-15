#include "Sdl2MouseInput.h"

#include <Sdl2Manager.h>
#include <MouseInputManager.h>

namespace flappy {

Sdl2MouseInput::Sdl2MouseInput()
{
    addDependency(Sdl2Manager::id());
    addDependency(MouseInputManager::id());

    subscribe([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_MOUSEMOTION) {
            manager<MouseInputManager>()->setMousePos(MouseInputManager::MouseButton::LEFT, {e.event.motion.x, e.event.motion.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONDOWN) {
            manager<MouseInputManager>()->setMouseDown(MouseInputManager::MouseButton::LEFT, {e.event.button.x, e.event.button.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONUP) {
            manager<MouseInputManager>()->setMouseUp(MouseInputManager::MouseButton::LEFT, {e.event.button.x, e.event.button.y});
        }
    });
}

} // flappy
