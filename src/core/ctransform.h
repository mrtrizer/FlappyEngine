#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CTransform
{
public:
    CTransform(float x, float y, float z = 0.0f): pos(x, y, z) {

    }

    glm::mat4x4 getMvMatrix() {
        auto translate = glm::translate(glm::mat4x4(1.0f), pos);
        auto rotate = glm::rotate(translate, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        return rotate;
    }

    glm::vec3 pos;
    float angle;
};

#endif // TRANSFORM_H
