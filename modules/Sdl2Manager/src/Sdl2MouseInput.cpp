#include "Sdl2MouseInput.h"

#include <Sdl2Manager.h>
#include <InputManager.h>

namespace flappy {

Sdl2MouseInput::Sdl2MouseInput(): Manager({Sdl2Manager::id()})
{
    events()->subscribeIn([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_MOUSEMOTION) {
            InputManager::OnMouseMove onMouseMoveEvent;
            onMouseMoveEvent.x = e.event.motion.x;
            onMouseMoveEvent.y = e.event.motion.y;
            events()->post(onMouseMoveEvent);
        }
        if (e.event.type == SDL_MOUSEBUTTONDOWN) {
            InputManager::OnMouseDown onMouseDownEvent;
            onMouseDownEvent.x = e.event.button.x;
            onMouseDownEvent.y = e.event.button.y;
            onMouseDownEvent.button = InputManager::MouseButton::LEFT;
            events()->post(onMouseDownEvent);
        }
    });
}

} // flappy
