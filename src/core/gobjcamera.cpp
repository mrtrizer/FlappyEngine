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

GTools::PMatrix GObjCamera::getPMatrix() const {
    auto rect = getRect();
    static const float near = -1.0f;
    static const float far = 99.0f;

    return GTools::PMatrix({
           2.0f / (rect.x2 - rect.x1), 0, 0, 0,
           0, 2.0f / (rect.y1 - rect.y2), 0, 0,
           0, 0, -2.0f / (far - near), 0,
           (rect.x2 + rect.x1) / (rect.x2 - rect.x1), (rect.y1 + rect.y2) / (rect.y1 - rect.y2), (far + near) / (far - near), 1.0f
    });
}
