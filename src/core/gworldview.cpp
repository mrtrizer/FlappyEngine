#include "gworldview.h"
#include "transform.h"

GWorldView::~GWorldView(){
}

void GWorldView::setGWorldModel(GWorldModelP gWorldModel) {
    this->gWorld = gWorldModel;
    updateSize();
}

void GWorldView::update(entityx::EntityManager &es, entityx::EventManager &events, entityx::TimeDelta dt) {

    GObjCamera* camera = nullptr;

    es.each<GObjCamera>([&camera, this](entityx::Entity entity, GObjCamera &curCamera){
        camera = &curCamera;
        if (updateSizeFlag) {
            updateSizeFlag = false;
            camera->resize(width, height);
        }
    });

    if (camera == nullptr)
        return;

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->getPMatrix();

    GPresenterList presenters;


    es.each<GPresenterRect,Transform>([&presenters](entityx::Entity entity, GPresenterRect &presenter, Transform gpos){
        presenters.push_back(Visual{std::make_shared<GPresenterRect>(presenter), gpos});
    });

    es.each<GPresenterSprite,Transform>([&presenters](entityx::Entity entity, GPresenterSprite &presenter, Transform gpos){
        presenters.push_back(Visual{std::make_shared<GPresenterSprite>(presenter), gpos});
    });

    redraw(presenters, pMatrix);
}

void GWorldView::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. Has to be > 0.");
    this->width = width;
    this->height = height;
    updateSize();
}

void GWorldView::updateSize() {
    updateSizeFlag = true;
    updateViewPort(width, height);
}
