#include "Sdl2MouseInput.h"

#include <Sdl2Manager.h>
#include <MouseInputManager.h>

namespace flappy {

Sdl2MouseInput::Sdl2MouseInput(Handle<Hierarchy> hierarchy)
    : m_sdl2Manager(hierarchy->manager<Sdl2Manager>())
    , m_mouseInputManager(hierarchy->manager<MouseInputManager>())
{
    m_subscription = m_sdl2Manager->eventBus().subscribe([this](Sdl2Manager::Sdl2Event e) {
        if (e.event.type == SDL_MOUSEMOTION) {
            m_mouseInputManager->setMousePos(MouseInputManager::MouseButton::LEFT, {e.event.motion.x, e.event.motion.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONDOWN) {
            m_mouseInputManager->setMouseDown(MouseInputManager::MouseButton::LEFT, {e.event.button.x, e.event.button.y});
        }
        if (e.event.type == SDL_MOUSEBUTTONUP) {
            m_mouseInputManager->setMouseUp(MouseInputManager::MouseButton::LEFT, {e.event.button.x, e.event.button.y});
        }
    });
}

} // flappy
