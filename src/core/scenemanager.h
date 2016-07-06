#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <core/manager.h>
#include <core/ccamera.h>
#include <core/prefab.h>
#include <functional>

class Scene {
public:
    virtual void init() = 0;
    virtual void update(TimeDelta dt) = 0;
};

class SceneManager: public Manager<SceneManager>
{
public:
    SceneManager();

    void update(TimeDelta dt) {
        if (curScene != nullptr)
            curScene->update(dt);
    }

    static void initPrefab(Prefab && prefab) {
        prefab.init();
    }

    static void loadScene(const std::shared_ptr<Scene>& scene) {
        EntityManager::getInst()->reset();
        scene->init();
        getInst()->curScene = scene;
    }

    static void setCamera(const std::shared_ptr<CCamera>& camera) {
        getInst()->camera = camera;
    }

    static std::shared_ptr<CCamera> getCamera() {
        return getInst()->camera;
    }

private:
    std::shared_ptr<Scene> curScene;
    std::shared_ptr<CCamera> camera;
};

#endif // SCENEMANAGER_H
