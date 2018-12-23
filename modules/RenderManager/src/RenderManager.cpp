#include "RenderManager.h"

#include <Updatable.hpp>
#include <SceneManager.h>
#include <ScreenManager.h>
#include <TransformComponent.h>
#include <Render.h>

namespace flappy {

using namespace std;
using namespace glm;

RenderManager::RenderManager(Handle<Hierarchy> hierarchy)
    : Updatable<RenderManager>(hierarchy)
    , m_sceneManager(hierarchy->manager<SceneManager>())
{}

void RenderManager::update(DeltaTime dt) {

    for (auto i = m_visuals.begin(); i !=  m_visuals.end(); i++) {
        auto& visual = *i;
        mat4 transformMatrix;
        float z = 0;
        auto curTransform = visual.view->entity()->component<TransformComponent>();
        while (curTransform != nullptr) {
            transformMatrix = curTransform->transformMatrix() * transformMatrix;
            z += curTransform->pos().z;
            auto entityPtr = curTransform->entity();
            auto parent = entityPtr->parent();
            if (parent.isValid())
                curTransform = parent->component<TransformComponent>();
            else
                curTransform = nullptr;
        }
        visual.pos = transformMatrix;
        visual.z = z;
    };

    auto camera = m_sceneManager->mainCamera();

    if (camera == nullptr)
        throw FlappyException("No main camera in scene!");

    auto pMatrix = camera->pMatrix();

    redraw(m_visuals, pMatrix);
}

void RenderManager::registerRenderElement(const Handle<Render> renderElement) {
    auto visual = Visual{renderElement, {}, 0};
    m_visuals.push_back(visual);
}

void RenderManager::unregisterRenderElement(const Handle<Render> renderElement) {
    m_visuals.remove_if([&renderElement](const Visual& i){ return i.view == renderElement; });
}

} // flappy
