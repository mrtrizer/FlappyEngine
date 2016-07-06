#include "gworldview.h"
#include "ctransform.h"
#include "gpresenter.h"
#include "screenmanager.h"
#include "scenemanager.h"

GWorldView* GWorldView::instance = nullptr;

GWorldView::~GWorldView(){
}

void GWorldView::update(TimeDelta dt) {

    std::shared_ptr<CCamera> camera;

    EntityManager::getInst()->each<CCamera>([&camera, this] (std::shared_ptr<Entity> e){
        camera = e->get<CCamera>();
    });

    if (camera == nullptr)
        throw no_camera();

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->getPMatrix();

    GPresenterList presenters;

    EntityManager::getInst()->each<GPresenter>([&presenters, dt]
                                    (std::shared_ptr<Entity> e){
        auto presenter = e->get<GPresenter>();
        presenter->update(dt);
        glm::mat4 transformMatrix;
        float z = 0;
        auto curTransform = e->get<CTransform>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos.z;
            curTransform = curTransform->parent.lock();
        }
        presenters.push_back(Visual{presenter, transformMatrix, z});
    });

    redraw(presenters, pMatrix);
}

void GWorldView::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. Has to be > 0.");
    ScreenManager::getInst()->width = width;
    ScreenManager::getInst()->height = height;
    updateSize();
}

void GWorldView::updateSize() {
    updateViewPort();
}
