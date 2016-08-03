#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include "transform.h"
#include "screenmgr.h"
#include "scenemgr.h"

namespace flappy {

Camera::Camera(float height, float ratio):
    m_height(height),
    m_ratio(ratio){

}

void Camera::init() {
    if (Scene::camera() == nullptr)
        Scene::setCamera(shared_from_this());
}

Camera::Rect Camera::rect() const {
    float ratio = (float)Screen::screenSize().x / Screen::screenSize().y;
    float offset = m_height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

vec3 Camera::screenToScene(const vec3 &pos) const {
    float coeff = this->m_height / Screen::screenSize().y;
    vec2 screenSize = Screen::screenSize() * 0.5f;
    vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

mat4 Camera::pMatrix() {
    auto curRect = rect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    mat4 mvMatrix;
    auto transform = entity()->get<Transform>();
    if (transform != nullptr) {
        mvMatrix = transform->getMvMatrix();
    }

    return ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far) * mvMatrix;
}

} // flappy
