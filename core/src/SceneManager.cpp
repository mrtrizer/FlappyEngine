#include "SceneManager.h"

#include "Scene.h"

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

    if (m_scene == nullptr) {
        m_scene = scene;
        m_scene->setParent(managerList());
        m_scene->init();
        m_nextScene = nullptr;
    } else {
        m_nextScene = scene;
        m_nextScene->setParent(managerList());
        m_nextScene->init();
    }
}


} // flappy
