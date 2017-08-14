#include "TransformComponent.h"

#include <glm/gtc/quaternion.hpp>

namespace flappy {

using namespace glm;

mat4x4 TransformComponent::transformMatrix() {
    auto translateM = translate(mat4x4(1.0f), m_pos);
    auto rotateM = translateM * glm::mat4_cast(m_quat);
    auto scaleM = glm::scale(rotateM, m_scale);
    return scaleM;
}

} // flappy
