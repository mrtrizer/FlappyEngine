#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <core/entitymanager.h>

class Transform: public Component, public std::enable_shared_from_this<Transform>
{
public:
    Transform(float x = 0, float y = 0, float z = 0.0f):
        pos(x, y, z){}

    void init() override {
        entity()->m_transform = shared_from_this();
    }

    glm::mat4x4 getMvMatrix() {
        auto translateM = glm::translate(glm::mat4x4(1.0f), pos);
        auto rotateM = glm::rotate(translateM, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        auto scaleM = glm::scale(rotateM, glm::vec3(scale, scale, scale));
        return scaleM;
    }

    glm::vec3 pos;
    float angle;
    float scale = 1.0f;
    std::weak_ptr<Transform> parent;
};

#endif // TRANSFORM_H
