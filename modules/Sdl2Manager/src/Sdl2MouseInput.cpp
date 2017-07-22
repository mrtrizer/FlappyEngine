#include "Sdl2MouseInput.h"

#include <Sdl2Manager.h>
#include <InputManager.h>

namespace flappy {

Sdl2MouseInput::Sdl2MouseInput(): Manager({Sdl2Manager::id()})
{
    events()->subscribeIn([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_MOUSEMOTION) {
            manager<InputManager>()->setMousePos({e.event.motion.x, e.event.motion.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONDOWN) {
            manager<InputManager>()->setMouseDown({e.event.button.x, e.event.button.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONUP) {
            manager<InputManager>()->setMouseUp({e.event.button.x, e.event.button.y});
        }
    });
}

} // flappy
