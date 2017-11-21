#include "RenderManager.h"

#include <Tools.h>
#include <SceneManager.h>
#include <ScreenManager.h>
#include <TransformComponent.h>
#include <Render.h>

namespace flappy {

using namespace std;
using namespace glm;

RenderManager::RenderManager()
{
    addDependency(SceneManager::id());

    subscribe([this](UpdateEvent e) {
        update(e.dt);
    });
}

void RenderManager::update(DeltaTime dt) {

    for (auto i = m_visuals.begin(); i !=  m_visuals.end(); i++) {
        auto& visual = *i;
        mat4 transformMatrix;
        float z = 0;
        auto curTransform = visual.view->entityRef()->component<TransformComponent>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->transformMatrix() * transformMatrix;
            z += curTransform->pos().z;
            auto entityPtr = curTransform->entityRef();
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

void RenderManager::registerRenderElement(const SafePtr<Render> renderElement) {
    auto visual = Visual{renderElement, {}, 0};
    m_visuals.push_back(visual);
}

void RenderManager::unregisterRenderElement(const SafePtr<Render> renderElement) {
    m_visuals.remove_if([&renderElement](const Visual& i){ return i.view == renderElement; });
}

} // flappy
