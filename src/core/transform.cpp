#include "transform.h"

namespace flappy {

using namespace glm;

void Transform::init() {
    entity()->m_transform = shared_from_this();
}

mat4x4 Transform::getMvMatrix() {
    auto translateM = translate(mat4x4(1.0f), m_pos);
    auto rotateM = rotate(translateM, m_angle, vec3(0.0f, 0.0f, 1.0f));
    auto scaleM = glm::scale(rotateM, m_scale);
    return scaleM;
}

} // flappy
