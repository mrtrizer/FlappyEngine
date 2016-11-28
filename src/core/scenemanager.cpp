#include "scenemanager.h"

#include "scene.h"

namespace flappy {

using namespace std;

void SceneManager::update(float) {
    if (m_nextScene != nullptr) {
        m_scene = m_nextScene;
        m_nextScene = nullptr;
    }
    m_scene->update();
}

void SceneManager::setScene(std::shared_ptr<Scene> scene) {
    m_nextScene = scene;
    if (m_nextScene != nullptr) {
        m_nextScene->setParent(managerList());
        m_nextScene->init();
    }
    if (m_scene == nullptr) {
        m_scene = m_nextScene;
        m_nextScene = nullptr;
    }
}


} // flappy
