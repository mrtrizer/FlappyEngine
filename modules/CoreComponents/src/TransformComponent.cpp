#include "TransformComponent.h"

namespace flappy {

using namespace glm;

mat4x4 TransformComponent::transformMatrix() {
    auto translateM = translate(mat4x4(1.0f), m_pos);
    auto rotateM = glm::rotate(translateM, m_angle, vec3(0.0f, 0.0f, 1.0f));
    auto scaleM = glm::scale(rotateM, m_scale);
    return scaleM;
}

} // flappy
