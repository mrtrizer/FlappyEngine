#pragma once

#include <list>

#include <core/managerlist.h>
#include <core/camera.h>

namespace flappy {

class Scene: public ManagerList {
public:
    virtual void init() {}
    void setCamera(const std::weak_ptr<Camera>& camera) { m_camera = camera; }
    std::shared_ptr<Camera> camera() const { return m_camera.lock(); }
    virtual std::list<std::shared_ptr<Scene>> sceneList() { return {}; }
private:
    std::weak_ptr<Camera> m_camera;
};

} // flappy
