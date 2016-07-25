#include <glm/gtc/matrix_transform.hpp>
#include <core/transform.h>

#include "camera.h"
#include "screen.h"
#include "scene.h"

Camera::Camera(float height, float ratio):
    height(height),
    ratio(ratio){

}

void Camera::init() {
    if (Scene::getInst()->getCamera() == nullptr)
        Scene::setCamera(shared_from_this());
}

Camera::Rect Camera::getRect() const {
    float ratio = (float)Screen::getInst()->width / Screen::getInst()->height;
    float offset = height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

glm::vec3 Camera::screenToScene(glm::vec3 pos) const {
    float coeff = this->height / Screen::getInst()->height;
    glm::vec2 screenSize = Screen::getInst()->getScreenSize() * 0.5f;
    glm::vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

glm::mat4 Camera::getPMatrix() {
    auto rect = getRect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    glm::mat4 mvMatrix;
    auto transform = entity()->get<Transform>();
    if (transform != nullptr) {
        mvMatrix = transform->getMvMatrix();
    }

    return glm::ortho(rect.x1, rect.x2, rect.y2, rect.y1, near, far) * mvMatrix;
}