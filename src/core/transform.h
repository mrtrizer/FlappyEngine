#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform(float x, float y, float z = 0.0f): pos(x, y, z) {

    }

    glm::mat4x4 getMvMatrix() {
        return glm::translate(glm::mat4x4(1.0f), pos);
    }

    glm::vec3 pos;
};

#endif // TRANSFORM_H
