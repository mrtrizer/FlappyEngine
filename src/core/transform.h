#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/entitymgr.h>

namespace flappy {

using namespace std;
using namespace glm;

class Transform: public Component, public enable_shared_from_this<Transform>
{
public:
    void init() override;
    mat4x4 getMvMatrix();

    vec3 pos() const {return m_pos;}
    float angle() const {return m_angle;}
    vec3 scale() const {return m_scale;}
    shared_ptr<Transform> parent() {return m_parent.lock();}

    void setPos(const vec3& pos) {m_pos = pos;}
    void move(const vec3& offset) {m_pos += offset;}

    void setAngle(float angle) {m_angle = angle;}

    void setScale(const vec3& scale) {m_scale = scale;}
    void setScale(float offset) {m_scale = vec3(offset, offset, offset);}
    void stretch(float offset) {m_scale += vec3(offset, offset, offset);}
    void stretch(const vec3& offset) {m_scale += offset;}

    void setParent(const shared_ptr<Transform>& parent) {m_parent = parent;}

private:
    vec3 m_pos = {0.0f, 0.0f, 0.0f};
    vec3 m_scale = {1.0f, 1.0f, 1.0f};
    float m_angle = 0.0f;
    weak_ptr<Transform> m_parent;
};

} // flappy
