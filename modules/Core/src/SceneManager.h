#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include <Manager.h>

#include "Entity.h"

namespace flappy
{

class CameraComponent;

class SceneManager: public Manager<SceneManager>
{
public:

    void regiser(SafePtr<Entity> entity);

    void unregiser(SafePtr<Entity> entity);

    template <typename ... Components>
    void each(std::function<void(SafePtr<Entity>)> func)
    {
        for (auto entity: m_entities) {
            if (check<Components...>(entity))
                func(entity);
        }
    }

    void setMainCamera(const SafePtr<CameraComponent>& camera);

    SafePtr<CameraComponent> mainCamera() const;

private:
    SafePtr<CameraComponent> m_camera;
    std::list<SafePtr<Entity>> m_entities;

    template <typename ComponentT = void, typename ... Components>
    bool check(std::shared_ptr<Entity> entity)
    {
        return check<Components...>(entity) && (entity->findComponent<ComponentT>());
    }
};

template <> inline
bool SceneManager::check <void> (std::shared_ptr<Entity>)
{
    return true;
}

} // flappy
