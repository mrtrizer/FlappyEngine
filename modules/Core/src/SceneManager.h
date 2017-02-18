#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include <Manager.h>

#include "Entity.h"

namespace flappy {

class CameraComponent;

class SceneManager: public Manager {
public:
    void update(TimeDelta dt);

    std::shared_ptr<Entity> add(std::shared_ptr<Entity>);

    template <typename ... Components>
    void each(std::function<void(std::shared_ptr<Entity>)> func) {
        for (auto entity: m_entities) {
            if (check<Components...>(entity))
                func(entity);
        }
    }

    void setCamera(const std::weak_ptr<CameraComponent>& camera) { m_camera = camera; }
    std::shared_ptr<CameraComponent> camera() const { return m_camera.lock(); }

private:
    std::weak_ptr<CameraComponent> m_camera;

    std::list<std::shared_ptr<Entity>> m_entities;

    template <typename ComponentT = void, typename ... Components>
    bool check(std::shared_ptr<Entity> entity) {
        return check<Components...>(entity) && (entity->findComponent<ComponentT>());
    }
};

template <> inline bool SceneManager::check <void> (std::shared_ptr<Entity>) {
    return true;
}

using EP = const std::shared_ptr<Entity>&;

} // flappy
