#include "ViewManager.h"

#include <Tools.h>
#include <SceneManager.h>
#include <ScreenManager.h>
#include <TransformComponent.h>
#include <View.h>

namespace flappy {

using namespace std;
using namespace glm;

ViewManager::ViewManager(std::list<unsigned> dependencies):
    Manager(Tools::concat(dependencies, {SceneManager::id()}))
{}

void ViewManager::update(DeltaTime dt) {

    for (auto i = m_visuals.begin(); i !=  m_visuals.end(); i++) {
        auto& visual = *i;
        mat4 transformMatrix;
        float z = 0;
        auto curTransform = visual.view->entity()->component<TransformComponent>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->transformMatrix() * transformMatrix;
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

    auto scene = manager<SceneManager>();
    if (scene == nullptr)
        return ERROR_MSG(VOID_VALUE, "No scene is set!\n");

    auto camera = scene->mainCamera();

    if (camera == nullptr)
        return ERROR_MSG(VOID_VALUE, "No main camera in scene!\n");

    auto pMatrix = camera->pMatrix();

    redraw(m_visuals, pMatrix);
}

void ViewManager::registerRenderElement(const SafePtr<View> renderElement) {
    auto visual = Visual{renderElement, {}, 0};
    m_visuals.push_back(visual);
}

} // flappy
