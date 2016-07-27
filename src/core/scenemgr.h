#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <core/manager.h>
#include <core/camera.h>
#include <core/prefab.h>
#include <functional>

class SceneMgr: public Manager<SceneMgr>
{
public:
    void initPrefab(Prefab && prefab);
    void setCamera(const std::shared_ptr<Camera>& camera);
    std::shared_ptr<Camera> camera();

private:
    std::shared_ptr<Camera> m_camera;
};

namespace Scene {
    inline void initPrefab(Prefab && prefab) {return SceneMgr::getInst()->initPrefab(std::move(prefab));}
    inline void setCamera(const std::shared_ptr<Camera>& camera) {SceneMgr::getInst()->setCamera(camera);}
    inline std::shared_ptr<Camera> camera() {return SceneMgr::getInst()->camera();}
}

#endif // SCENEMANAGER_H
