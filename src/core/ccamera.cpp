#include <glm/gtc/matrix_transform.hpp>

#include "ccamera.h"
#include "gcontext.h"

CCamera::CCamera(float height, float ratio, int windowHeight):
    height(height),
    ratio(ratio),
    coeff(height / windowHeight) {
}

CCamera::Rect CCamera::getRect() const {
    float offset = height / 2;
    return {
        -offset * ratio,
        offset,
        offset * ratio,
        -offset,
    };
}

void CCamera::resize(double width, double height) {
    this->ratio = width / height;
    this->coeff = this->height / height;
}

glm::mat4 CCamera::getPMatrix() const {
    auto rect = getRect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    return glm::ortho(rect.x1, rect.x2, rect.y2, rect.y1, near, far);
}
