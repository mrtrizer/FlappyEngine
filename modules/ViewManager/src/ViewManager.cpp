#include "ViewManager.h"

#include <SceneManager.h>
#include <ScreenManager.h>
#include <TransformComponent.h>
#include <PresenterComponent.h>

namespace flappy {

using namespace std;
using namespace glm;

void ViewManager::update(TimeDelta dt) {

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
        auto curTransform = visual.presenter->entity()->component<TransformComponent>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->getMvMatrix() * transformMatrix;
            z += curTransform->pos().z;
            auto entityPtr = curTransform->entity();
            if (auto parentPtr = entityPtr->parent())
                curTransform = parentPtr->component<TransformComponent>();
            else
                curTransform = nullptr;
        }
        visual.pos = transformMatrix;
        visual.z = z;
    };

    auto scene = component<SceneManager>();
    if (scene == nullptr)
        return ERROR_MSG(VOID_VALUE, "No scene is set!\n");

    auto camera = scene->camera();

    if (camera == nullptr)
        return ERROR_MSG(VOID_VALUE, "No main camera in scene!\n");

    auto pMatrix = camera->pMatrix();

    redraw(m_visuals, pMatrix);
}

void ViewManager::addPresenter(const std::shared_ptr<PresenterComponent>& presenter) {
    unsigned id = presenter->id();
    if (m_bindings[id] == nullptr)
        return ERROR_MSG(VOID_VALUE, "PresenterComponent component %d was not binded.", id);
    auto visual = Visual{presenter, m_bindings[id]->build(), {}, 0};
    presenter->setView(visual.view);
    m_visuals.push_back(visual);
}

void ViewManager::resize(int width, int height) {
    if (width < 1 || height < 1)
        throw runtime_error("Invalid screen size. It should be greater then 0.");
    manager<ScreenManager>()->resize(width, height);
    updateViewPort();
}

} // flappy
