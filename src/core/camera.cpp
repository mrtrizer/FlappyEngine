#include <glm/gtc/matrix_transform.hpp>
#include <core/transform.h>

#include "camera.h"
#include "screenmgr.h"
#include "scenemgr.h"

Camera::Camera(float height, float ratio):
    m_height(height),
    m_ratio(ratio){

}

void Camera::init() {
    if (Scene::camera() == nullptr)
        Scene::setCamera(shared_from_this());
}

Camera::Rect Camera::rect() const {
    float ratio = (float)ScreenMgr::getInst()->width() / ScreenMgr::getInst()->height();
    float offset = m_height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

glm::vec3 Camera::screenToScene(glm::vec3 pos) const {
    float coeff = this->m_height / ScreenMgr::getInst()->height();
    glm::vec2 screenSize = ScreenMgr::getInst()->screenSize() * 0.5f;
    glm::vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

glm::mat4 Camera::pMatrix() {
    auto curRect = rect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    glm::mat4 mvMatrix;
    auto transform = entity()->get<Transform>();
    if (transform != nullptr) {
        mvMatrix = transform->getMvMatrix();
    }

    return glm::ortho(curRect.x1, curRect.x2, curRect.y2, curRect.y1, near, far) * mvMatrix;
}
