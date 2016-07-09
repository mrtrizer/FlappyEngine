#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <core/manager.h>
#include <core/camera.h>
#include <core/prefab.h>
#include <functional>

class Scene: public Manager<Scene>
{
public:
    Scene();

    static void initPrefab(Prefab && prefab) {
        prefab.init();
    }

    static void setCamera(const std::shared_ptr<Camera>& camera) {
        getInst()->camera = camera;
    }

    static std::shared_ptr<Camera> getCamera() {
        return getInst()->camera;
    }

private:
    std::shared_ptr<Camera> camera;
};

#endif // SCENEMANAGER_H
