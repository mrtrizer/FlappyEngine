#include "viewmanager.h"
#include "transform.h"
#include "presenter.h"
#include "screenmgr.h"
#include "scenemgr.h"

ViewManager* ViewManager::instance = nullptr;

ViewManager::~ViewManager(){
}

void ViewManager::update(TimeDelta dt) {

    std::shared_ptr<Camera> camera;

    EntityMgr::getInst()->each<Camera>([&camera, this] (EP e){
        camera = e->get<Camera>();
    });

    if (camera == nullptr)
        throw no_camera();

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->getPMatrix();

    GPresenterList presenters;

    EntityMgr::getInst()->each<Presenter>([&presenters, dt] (EP e){
        auto presenter = e->get<Presenter>();
        presenter->update(dt);
        glm::mat4 transformMatrix;
        float z = 0;
        auto curTransform = e->get<Transform>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos.z;
            curTransform = curTransform->parent.lock();
        }
        presenters.push_back(Visual{presenter, transformMatrix, z});
    });

    redraw(presenters, pMatrix);
}

void ViewManager::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. Has to be > 0.");
    ScreenMgr::getInst()->resize(width, height);
    updateSize();
}

void ViewManager::updateSize() {
    updateViewPort();
}
