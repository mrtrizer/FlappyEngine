#include "gworldview.h"
#include "ctransform.h"
#include "cpresenter.h"
#include "screenmanager.h"

GWorldView::~GWorldView(){
}

void GWorldView::setGWorldModel(GWorldModelP gWorldModel) {
    this->gWorld = gWorldModel;
    updateSize();
}

void GWorldView::update(entityx::EntityManager &es, entityx::EventManager&, entityx::TimeDelta dt) {

    CCamera* camera = nullptr;
    CTransform* cameraTransform = nullptr;

    es.each<CCamera, CTransform>([&camera, &cameraTransform, this]
                                 (entityx::Entity, CCamera &curCamera, CTransform& transform){
        camera = &curCamera;
        cameraTransform = &transform;
    });

    if (camera == nullptr)
        throw no_camera();

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->getPMatrix();
    pMatrix = glm::translate(pMatrix, cameraTransform->pos);
    pMatrix = glm::rotate(pMatrix, cameraTransform->angle, glm::vec3(0.0f, 0.0f, 1.0f));

    GPresenterList presenters;

    es.each<CPresenter, CTransform>([&presenters, dt]
                                    (entityx::Entity entity, CPresenter &cpresenter, CTransform gpos){
        auto presenter = cpresenter.getPresenter();
        presenter->update(dt);
        presenters.push_back(Visual{presenter, gpos});
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
