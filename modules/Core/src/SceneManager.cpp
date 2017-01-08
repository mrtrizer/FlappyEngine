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

    scene->setParent(managerList());
    scene->create<EntityManager>();
    scene->init();
    if (m_scene == nullptr) {
        m_scene = scene;
        m_nextScene = nullptr;
    } else {
        m_nextScene = scene;
    }
}


} // flappy
