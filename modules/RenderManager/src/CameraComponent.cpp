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

CameraComponent::CameraComponent(Handle<Entity> entity)
    : m_sceneManager(entity->hierarchy()->manager<SceneManager>())
    , m_screenManager(entity->hierarchy()->manager<ScreenManager>())
    , m_transformComponent(entity->component<TransformComponent>())
{
    m_sceneManager->setMainCamera(selfHandle());
}
    
MathUtils::Rect CameraComponent::rect() const {
    auto screenSize = glm::vec2(m_screenManager->screenSize());
    float yOffset = m_height / 2;
    float xOffset = (yOffset * screenSize.x) / screenSize.y;
    return { -xOffset, yOffset, xOffset, -yOffset };
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
