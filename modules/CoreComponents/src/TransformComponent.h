#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Component.h>

namespace flappy
{

class TransformComponent: public Component<TransformComponent>
{
public:
    glm::mat4x4 transformMatrix();

    glm::vec3 pos() const {return m_pos;}
    float angle2DRad() const {return glm::axis(m_quat).z;}
    glm::vec3 scale() const {return m_scale;}

    void setPos(const glm::vec3& pos) {m_pos = pos;}
    void move(const glm::vec2& offset) {m_pos += glm::vec3(offset, 0.0f);}
    void move(const glm::vec3& offset) {m_pos += offset;}

    void rotate(float angle) {m_quat = glm::rotate(m_quat, angle, glm::vec3(0.0f, 0.0f, 1.0f));}

    void setAngle2DRad(float angle) {m_quat = glm::quat(glm::vec3(0.0f, 0.0f, angle));}

    void setScale(const glm::vec3& scale) {m_scale = scale;}
    void setScale(const glm::vec2& scale) {m_scale = glm::vec3(scale, 0.0f);}
    void setScale(float offset) {m_scale = glm::vec3(offset, offset, offset);}
    void stretch(float offset) {m_scale += glm::vec3(offset, offset, offset);}
    void stretch(const glm::vec3& offset) {m_scale += offset;}

private:
    glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
    glm::quat m_quat;
};

} // flappy
