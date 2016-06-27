#include <glm/gtc/matrix_transform.hpp>
#include <core/ctransform.h>

#include "ccamera.h"
#include "screenmanager.h"

CCamera::CCamera(const entityx::Entity& e, float height, float ratio):
    height(height),
    ratio(ratio),
    Component(e){
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
    entityx::ComponentHandle<CTransform> transform = e.component<CTransform>();
    if (transform.valid()) {
        mvMatrix = transform->getMvMatrix();
    }

    return glm::ortho(rect.x1, rect.x2, rect.y2, rect.y1, near, far) * mvMatrix;
}
