#include "scenemgr.h"

#include "flappyapp.h"

void SceneMgr::initPrefab(Prefab && prefab) {
    prefab.init();
}

void SceneMgr::setCamera(const std::shared_ptr<Camera>& camera) {
    m_camera = camera;
}

std::shared_ptr<Camera> SceneMgr::camera() {
    return m_camera;
}

namespace Scene {
    void initPrefab(Prefab && prefab) {
        return FlappyApp::inst().sceneMgr()->initPrefab(std::move(prefab));
    }

    void setCamera(const std::shared_ptr<Camera>& camera) {
        FlappyApp::inst().sceneMgr()->setCamera(camera);
    }

    std::shared_ptr<Camera> camera() {
        return FlappyApp::inst().sceneMgr()->camera();
    }
}
