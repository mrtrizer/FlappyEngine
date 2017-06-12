#include "SceneManager.h"

namespace flappy {

void SceneManager::setMainCamera(const SafePtr<CameraComponent>& camera)
{
    m_camera = camera;
}

SafePtr<CameraComponent> SceneManager::mainCamera() const
{
    return m_camera;
}

}
