#pragma once

#include <core/manager.h>
#include <core/camera.h>
#include <core/prefab.h>
#include <functional>

namespace flappy {

using namespace std;

class SceneMgr: public Manager<SceneMgr>
{
public:
    void initPrefab(Prefab && prefab);
    void setCamera(const shared_ptr<Camera>& camera);
    shared_ptr<Camera> camera();

private:
    shared_ptr<Camera> m_camera;
};

} // flappy
