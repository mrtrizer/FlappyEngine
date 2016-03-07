#include "gobjcamera.h"
#include "gcontext.h"

GObjCamera::GObjCamera(float height, float ratio, int windowHeight, const GPos & pos):
    GObj(pos),
    height(height),
    ratio(ratio),
    coeff(height / windowHeight) {
}

GObjCamera::Rect GObjCamera::getRect() const {
    GPos pos = getAPos();
    float offset = height / 2;
    return {
        -offset * ratio - pos.getX(),
        offset - pos.getY(),
        offset * ratio - pos.getX(),
        -offset - pos.getY(),
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

void GObjCamera::init() {
    gObjPointer = ADD_CHILD(GObjPointer);
}

void GObjCamera::recalc(GObj::DeltaT, const GContext & context) {
    float width = height * ratio;
    float x = (float)context.getX() * coeff - width / 2;
    float y = -((float)context.getY() * coeff - height / 2);
    gObjPointer->setPos(GPos(x,y,0));
}
