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

    for (auto i = m_visuals.begin(); i !=  m_visuals.end(); ) {
        auto& visual = *i;
        if (visual.presenter.use_count() == 1) {
            i = m_visuals.erase(i);
            continue;
        } else {
            i++;
        }
        visual.presenter->update(dt);
        mat4 transformMatrix;
        float z = 0;
        auto curTransform = visual.presenter->entity()->get<Transform>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos().z;
            curTransform = curTransform->parent();
        }
        visual.pos = transformMatrix;
        visual.z = z;
    };

    redraw(m_visuals, pMatrix);
}

void ViewManager::addPresenter(const std::shared_ptr<Presenter>& presenter) {
    unsigned id = presenter->id();
    if (m_bindings.size() <= id)
        return ERROR_MSG(VOID_VALUE, "Presenter component %d was not binded.", id);
    if (m_bindings[id] == nullptr)
        return ERROR_MSG(VOID_VALUE, "Presenter component %d was not binded.", id);
    auto visual = Visual{presenter, m_bindings[id]->build(), {}, 0};
    presenter->setView(visual.view);
    m_visuals.push_back(visual);
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
