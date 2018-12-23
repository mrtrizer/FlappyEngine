#include "CameraComponent.h"

#include <glm/gtc/matrix_transform.hpp>

#include <TransformComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>
#include <Hierarchy.hpp>
#include <Entity.hpp>

namespace flappy {

using namespace std;
using namespace glm;

CameraComponent::CameraComponent(Handle<Hierarchy> hierarchy)
    : m_sceneManager(hierarchy->manager<SceneManager>())
    , m_screenManager(hierarchy->manager<ScreenManager>())
{
    m_sceneManager->setMainCamera(selfHandle());
}
    
void CameraComponent::setEntity(Handle<Entity> entity) {
    m_transformComponent = entity->component<TransformComponent>();
}
    
MathUtils::Rect CameraComponent::rect() const {
    auto screenSize = m_screenManager->screenSize();
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
    float coeff = this->m_height / m_screenManager->screenSize().y;
    vec2 screenSize = glm::vec2(m_screenManager->screenSize()) * 0.5f;
    vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

mat4 CameraComponent::pMatrix() {
    auto curRect = rect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    mat4 mvMatrix = m_transformComponent->transformMatrix();

    return ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far) * mvMatrix;
}

} // flappy
