#include "scenemgr.h"

#include <core/camera.h>

namespace flappy {

using namespace std;

void SceneMgr::setScene(const shared_ptr<Scene>& scene) {
    m_scene = scene;
    m_scene->setParent(managerList());
    m_scene->init();
}

} // flappy
