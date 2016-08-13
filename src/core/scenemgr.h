#pragma once

#include <core/manager.h>
#include <core/camera.h>
#include <core/prefab.h>
#include <functional>

namespace flappy {

class SceneMgr: public Manager<SceneMgr>
{
public:
    void initPrefab(Prefab && prefab);
    void setCamera(const std::shared_ptr<Camera>& camera);
    std::shared_ptr<Camera> camera();

private:
    std::shared_ptr<Camera> m_camera;
};

} // flappy
