#include "SceneManager.h"

namespace flappy {

void SceneManager::regiser(SafePtr<Entity> entity)
{
    m_entities.push_back(entity);
}

void SceneManager::unregiser(SafePtr<Entity> entity)
{
    m_entities.remove(entity);
}

void SceneManager::setMainCamera(const SafePtr<CameraComponent>& camera)
{
    m_camera = camera;
}

SafePtr<CameraComponent> SceneManager::mainCamera() const
{
    return m_camera;
}

}
