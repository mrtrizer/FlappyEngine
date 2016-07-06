#include <glm/gtc/matrix_transform.hpp>
#include <core/ctransform.h>

#include "ccamera.h"
#include "screenmanager.h"
#include "scenemanager.h"

CCamera::CCamera(float height, float ratio):
    height(height),
    ratio(ratio){

}

void CCamera::init() {
    if (SceneManager::getInst()->getCamera() == nullptr)
        SceneManager::setCamera(shared_from_this());
}

CCamera::Rect CCamera::getRect() const {
    float ratio = (float)ScreenManager::getInst()->width / ScreenManager::getInst()->height;
    float offset = height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

glm::vec3 CCamera::screenToScene(glm::vec3 pos) const {
    float coeff = this->height / ScreenManager::getInst()->height;
    glm::vec2 screenSize = ScreenManager::getInst()->getScreenSize() * 0.5f;
    glm::vec3 scenePos(pos.x - screenSize.x, screenSize.y - pos.y, 0);
    return scenePos * coeff;
}

glm::mat4 CCamera::getPMatrix() {
    auto rect = getRect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    glm::mat4 mvMatrix;
    auto transform = getEntity()->get<CTransform>();
    if (transform != nullptr) {
        mvMatrix = transform->getMvMatrix();
    }

    return glm::ortho(rect.x1, rect.x2, rect.y2, rect.y1, near, far) * mvMatrix;
}
