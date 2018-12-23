#include "SceneManager.h"

namespace flappy {

void SceneManager::setMainCamera(const Handle<CameraComponent>& camera)
{
    m_camera = camera;
}

} // flappy
