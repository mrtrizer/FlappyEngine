#include "scenemgr.h"

#include "flappyapp.h"

namespace flappy {

void SceneMgr::initPrefab(Prefab && prefab) {
    prefab.init();
}

void SceneMgr::setCamera(const shared_ptr<Camera>& camera) {
    m_camera = camera;
}

shared_ptr<Camera> SceneMgr::camera() {
    return m_camera;
}

namespace Scene {
    void initPrefab(Prefab && prefab) {
        return FlappyApp::inst().sceneMgr()->initPrefab(move(prefab));
    }

    void setCamera(const shared_ptr<Camera>& camera) {
        FlappyApp::inst().sceneMgr()->setCamera(camera);
    }

    shared_ptr<Camera> camera() {
        return FlappyApp::inst().sceneMgr()->camera();
    }
}

} // flappy
