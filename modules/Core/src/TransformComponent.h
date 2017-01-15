#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <EntityManager.h>

namespace flappy {

class TransformComponent: public Component, public std::enable_shared_from_this<TransformComponent>
{
public:
    void init() override;
    glm::mat4x4 getMvMatrix();

    glm::vec3 pos() const {return m_pos;}
    float angle() const {return m_angle;}
    glm::vec3 scale() const {return m_scale;}

    void setPos(const glm::vec3& pos) {m_pos = pos;}
    void move(const glm::vec2& offset) {m_pos += glm::vec3(offset, 0.0f);}
    void move(const glm::vec3& offset) {m_pos += offset;}

    void rotate(float angle) {m_angle += angle;}

    void setAngle(float angle) {m_angle = angle;}

    void setScale(const glm::vec3& scale) {m_scale = scale;}
    void setScale(const glm::vec2& scale) {m_scale = glm::vec3(scale, 0.0f);}
    void setScale(float offset) {m_scale = glm::vec3(offset, offset, offset);}
    void stretch(float offset) {m_scale += glm::vec3(offset, offset, offset);}
    void stretch(const glm::vec3& offset) {m_scale += offset;}

private:
    glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
    float m_angle = 0.0f;
};

} // flappy
