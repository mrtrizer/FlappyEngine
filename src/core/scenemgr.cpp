#include "scenemgr.h"

void SceneMgr::initPrefab(Prefab && prefab) {
    prefab.init();
}

void SceneMgr::setCamera(const std::shared_ptr<Camera>& camera) {
    m_camera = camera;
}

std::shared_ptr<Camera> SceneMgr::camera() {
    return m_camera;
}
