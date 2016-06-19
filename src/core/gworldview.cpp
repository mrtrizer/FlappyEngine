#include "gworldview.h"
#include "ctransform.h"
#include "cpresenter.h"
#include "screenmanager.h"
#include "scenemanager.h"

GWorldView::~GWorldView(){
}

void GWorldView::setGWorldModel(GWorldModelP gWorldModel) {
    this->gWorld = gWorldModel;
    updateSize();
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

    es.each<CPresenter, CTransform>([&presenters, dt]
                                    (entityx::Entity entity, CPresenter &cpresenter, CTransform gpos){
        auto presenter = cpresenter.getImpl();
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
