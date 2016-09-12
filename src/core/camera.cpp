#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>

#include "transform.h"
#include "screenmanager.h"
#include "scenemanager.h"
#include "scene.h"

namespace flappy {

using namespace std;
using namespace glm;
using namespace Tools;

Camera::Camera(float height):
    m_height(height){

}

void Camera::init() {
    if (auto scene = MGR<SceneManager>()->scene())
        if (scene->camera() == nullptr)
            scene->setCamera(shared_from_this());
}

Rect Camera::rect() const {
    float ratio = (float)MGR<ScreenManager>()->screenSize().x / MGR<ScreenManager>()->screenSize().y;
    float offset = m_height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

vec3 Camera::screenToScene(const vec2 &pos) const {
    float coeff = this->m_height / MGR<ScreenManager>()->screenSize().y;
    vec2 screenSize = MGR<ScreenManager>()->screenSize() * 0.5f;
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
