#pragma once

#include <core/managerlist.h>
#include <core/camera.h>

namespace flappy {

class Scene: public ManagerList {
public:
    virtual ~Scene(){}
    virtual void init() = 0;
    void setCamera(const std::shared_ptr<Camera>& camera) { m_camera = camera; }
    std::shared_ptr<Camera> camera() const { return m_camera; }
private:
    std::shared_ptr<Camera> m_camera;
};

} // flappy
