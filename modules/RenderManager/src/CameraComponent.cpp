#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include <TransformComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>

namespace flappy {

using namespace std;
using namespace glm;
using namespace Tools;

void CameraComponent::init() {
    events()->subscribeIn([this](ManagerAddedEvent e) {
        if (e.castTo<SceneManager>() != nullptr) {
            manager<SceneManager>()->setMainCamera(selfPointer<CameraComponent>());
        }
    });
}

Rect CameraComponent::rect() const {
    auto screenManager = manager<ScreenManager>();
    if (screenManager == nullptr)
        return {0,0,0,0};
    auto screenSize = screenManager->screenSize();
    float ratio = screenSize.x / screenSize.y;
    float offset = m_height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

vec3 CameraComponent::screenToScene(const vec2 &pos) const {
    float coeff = this->m_height / manager<ScreenManager>()->screenSize().y;
    vec2 screenSize = manager<ScreenManager>()->screenSize() * 0.5f;
    vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

mat4 CameraComponent::pMatrix() {
    auto curRect = rect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    mat4 mvMatrix;
    auto transform = entity()->component<TransformComponent>();
    if (transform != nullptr)
        mvMatrix = transform->transformMatrix();

    return ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far) * mvMatrix;
}

} // flappy
