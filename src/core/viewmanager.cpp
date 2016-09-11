#include "viewmanager.h"
#include "transform.h"
#include "presenter.h"
#include "screenmanager.h"
#include "scenemanager.h"

namespace flappy {

using namespace std;
using namespace glm;

void ViewManager::update(TimeDelta dt) {

    shared_ptr<Camera> camera;

    MGR<EntityManager>()->each<Camera>([&camera, this] (EP e){
        camera = e->get<Camera>();
    });

    if (camera == nullptr) {
        LOGI("No any active cameras on scene!\n");
        return;
    }

    //Calc projection matrix, using GObjCamera
    auto pMatrix = camera->pMatrix();

    list<Visual> presenters;

    MGR<EntityManager>()->each<Presenter>([&presenters, dt] (EP e){
        auto presenter = e->get<Presenter>();
        presenter->update(dt);
        mat4 transformMatrix;
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

void ViewManager::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw runtime_error("Invalid screen size. Has to be > 0.");
    MGR<ScreenManager>()->resize(width, height);
    updateSize();
}

void ViewManager::updateSize() {
    updateViewPort();
}

} // flappy
