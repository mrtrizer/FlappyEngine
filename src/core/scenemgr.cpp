#include "scenemgr.h"

#include "flappyapp.h"

namespace flappy {

using namespace std;

void SceneMgr::initPrefab(Prefab && prefab) {
    prefab.init();
}

void SceneMgr::setCamera(const shared_ptr<Camera>& camera) {
    m_camera = camera;
}

shared_ptr<Camera> SceneMgr::camera() {
    return m_camera;
}

} // flappy
