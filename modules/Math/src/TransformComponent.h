#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <Handle.hpp>

namespace flappy
{
    
class Entity;
class Hierarchy;

class [[component]] TransformComponent
{
public:
    TransformComponent(Handle<Hierarchy>) {}
    
    void setEntity(Handle<Entity> entity) { m_entity = entity; }
    
    Handle<Entity> entity() { return m_entity; }
    
    glm::mat4x4 transformMatrix();

    void setPos(const glm::vec3& pos) {m_pos = pos;}
    void move(const glm::vec2& offset) {m_pos += glm::vec3(offset, 0.0f);}
    void move(const glm::vec3& offset) {m_pos += offset;}
    const glm::vec3& pos() const {return m_pos;}

    void setRotation(glm::quat quat) {m_quat = quat;}
    void setEulerAngles(glm::vec3 angles) {m_quat = glm::quat(angles); }
    void setAngle2DRad(float angle) {m_quat = glm::quat(glm::vec3(0.0f, 0.0f, angle));}
    void rotate2DRad(float angle) {m_quat = glm::rotate(m_quat, angle, glm::vec3(0.0f, 0.0f, 1.0f));}
    float angle2DRad() const {return glm::eulerAngles(m_quat).z;}
    glm::vec3 eulerAngles() {return glm::eulerAngles(m_quat);}
    const glm::quat& rotation() const { return m_quat; }

    void setScale(const glm::vec3& scale) {m_scale = scale;}
    void setScale(const glm::vec2& scale) {m_scale = glm::vec3(scale, 0.0f);}
    void setScale(float offset) {m_scale = glm::vec3(offset, offset, offset);}
    void stretch(float offset) {m_scale += glm::vec3(offset, offset, offset);}
    void stretch(const glm::vec3& offset) {m_scale += offset;}
    const glm::vec3& scale() const {return m_scale;}

private:
    glm::vec3 m_pos = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_scale = {1.0f, 1.0f, 1.0f};
    glm::quat m_quat;
    Handle<Entity> m_entity;
};

} // flappy
