#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <entityx/entityx.h>
#include <core/manager.h>
#include <core/ccamera.h>
#include <core/prefab.h>
#include <functional>

class EntityHandler {
public:
    EntityHandler(const entityx::Entity&& entity = entityx::Entity(), entityx::EntityManager* em = nullptr):entity(entity), em(em)
    {}

    template<typename ComponentT, typename ComponentImpl, typename ... Args>
    std::shared_ptr<ComponentImpl> add(Args ... args) {
        auto impl = std::make_shared<ComponentImpl>(args...);
        entity.assign<ComponentT>(entity)->setImpl(impl);
        return impl;
    }

    template<typename ComponentT, typename ... Args>
    entityx::ComponentHandle<ComponentT> add(Args ... args) {
        return entity.assign<ComponentT>(entity, args...);
    }

    bool isValid() {
        return entity.valid();
    }

    entityx::Entity getEntity() const {
        return entity;
    }

private:
    entityx::Entity entity;
    entityx::EntityManager* em;
};

class Scene {
public:
    virtual void init() = 0;
    virtual void update(entityx::TimeDelta dt) = 0;
};

class SceneManager: public Manager<SceneManager>
{
public:
    SceneManager();
    void init(entityx::EntityManager* entities) {
        this->entities = entities;
    }

    void update(entityx::TimeDelta dt) {
        if (curScene != nullptr)
            curScene->update(dt);
    }

    static void initPrefab(Prefab && prefab) {
        prefab.init(*getInst()->entities);
    }

    static EntityHandler&& createEntity() {
        return EntityHandler(getInst()->entities->create(), getInst()->entities);
    }

    static void loadScene(const std::shared_ptr<Scene>& scene) {
        getInst()->entities->reset();
        scene->init();
        getInst()->curScene = scene;
    }

    static void setMainCamera(const entityx::ComponentHandle<CCamera>& camera) {
        getInst()->camera = camera;
    }

    static entityx::ComponentHandle<CCamera> getMainCamera() {
        return getInst()->camera;
    }

    static entityx::EntityManager* getEMgr() {return getInst()->entities;}

private:
    entityx::EntityManager* entities;
    std::shared_ptr<Scene> curScene;
    entityx::ComponentHandle<CCamera> camera;
};

#endif // SCENEMANAGER_H
