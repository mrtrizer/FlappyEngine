#include <glm/gtc/matrix_transform.hpp>

#include "gobjcamera.h"
#include "gcontext.h"

GObjCamera::GObjCamera(float height, float ratio, int windowHeight):
    height(height),
    ratio(ratio),
    coeff(height / windowHeight) {
}

GObjCamera::Rect GObjCamera::getRect() const {
    float offset = height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

void GObjCamera::resize(double width, double height) {
    this->ratio = width / height;
    this->coeff = this->height / height;
}

glm::mat4 GObjCamera::getPMatrix() const {
    auto rect = getRect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    return glm::ortho(rect.x1, rect.x2, rect.y2, rect.y1, near, far);
}
