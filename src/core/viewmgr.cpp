#include "viewmgr.h"
#include "transform.h"
#include "presenter.h"
#include "screenmgr.h"
#include "scenemgr.h"`

void ViewMgr::update(TimeDelta dt) {

    std::shared_ptr<Camera> camera;

    EM::each<Camera>([&camera, this] (EP e){
        camera = e->get<Camera>();
    });

    if (camera == nullptr)
        throw no_camera();

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->pMatrix();

    std::list<Visual> presenters;

    EM::each<Presenter>([&presenters, dt] (EP e){
        auto presenter = e->get<Presenter>();
        presenter->update(dt);
        glm::mat4 transformMatrix;
        float z = 0;
        auto curTransform = e->get<Transform>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos().z;
            curTransform = curTransform->parent();
        }
        presenters.push_back(Visual{presenter, transformMatrix, z});
    });

    redraw(presenters, pMatrix);
}

void ViewMgr::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw std::runtime_error("Invalid screen size. Has to be > 0.");
    FlappyApp::inst().screenMgr()->resize(width, height);
    updateSize();
}

void ViewMgr::updateSize() {
    updateViewPort();
}
