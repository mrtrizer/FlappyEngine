#include "SceneManager.h"

namespace flappy {

void SceneManager::setMainCamera(Handle<CameraComponent> camera)
{
    m_camera = camera;
    m_eventBus.post(CameraChangedEvent{});
}

} // flappy
