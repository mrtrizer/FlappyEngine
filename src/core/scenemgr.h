#pragma once

#include <core/managerlist.h>
#include <core/manager.h>
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

class SceneMgr: public Manager<SceneMgr>
{
public:
    void setScene(const std::shared_ptr<Scene>& scene);
    std::shared_ptr<Scene> scene() const { return m_scene; }
    std::shared_ptr<Camera> camera() const { return m_scene->camera(); } //TODO: Remove this method
    void setCamera(const std::shared_ptr<Camera>& camera) {m_scene->setCamera(camera);} //TODO: Remove this method

private:
    std::shared_ptr<Scene> m_scene;
};

} // flappy
