#include "gworldview.h"
#include "ctransform.h"
#include "cpresenter.h"
#include "screenmanager.h"
#include "scenemanager.h"

GWorldView* GWorldView::instance = nullptr;

GWorldView::~GWorldView(){
}

void GWorldView::update(entityx::EntityManager &es, entityx::EventManager&, entityx::TimeDelta dt) {

    CCamera* camera = nullptr;

    es.each<CCamera>([&camera, this] (entityx::Entity, CCamera &curCamera){
        camera = &curCamera;
    });

    if (camera == nullptr)
        throw no_camera();

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->getPMatrix();

    GPresenterList presenters;

    es.each<CPresenter>([&presenters, dt]
                                    (entityx::Entity entity, CPresenter &cpresenter){
        auto presenter = cpresenter.getImpl();
        presenter->update(dt);
        glm::mat4 transformMatrix;
        CTransform* curTransform = nullptr;
        float z = 0;
        auto componentHandle = entity.component<CTransform>();
        if (componentHandle.valid())
            curTransform = componentHandle.get();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos.z;
            curTransform = curTransform->parent;
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
