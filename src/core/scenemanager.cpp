#include "scenemanager.h"

#include "scene.h"

namespace flappy {

using namespace std;

void SceneManager::setScene(const shared_ptr<Scene>& scene) {
    m_scene = scene;
    m_scene->setParent(managerList());
    m_scene->init();
}

} // flappy
