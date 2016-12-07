#pragma once

#include <list>

#include <ManagerList.h>
#include <CameraComponent.h>

namespace flappy {

class Scene: public ManagerList {
public:
    virtual void init() {}
    void setCamera(const std::weak_ptr<CameraComponent>& camera) { m_camera = camera; }
    std::shared_ptr<CameraComponent> camera() const { return m_camera.lock(); }
    virtual std::list<std::shared_ptr<Scene>> sceneList() { return {}; }
private:
    std::weak_ptr<CameraComponent> m_camera;
};

} // flappy
