#include "transform.h"

void Transform::init() {
    entity()->m_transform = shared_from_this();
}

glm::mat4x4 Transform::getMvMatrix() {
    auto translateM = glm::translate(glm::mat4x4(1.0f), m_pos);
    auto rotateM = glm::rotate(translateM, m_angle, glm::vec3(0.0f, 0.0f, 1.0f));
    auto scaleM = glm::scale(rotateM, m_scale);
    return scaleM;
}
