#ifndef CCAMERA_H
#define CCAMERA_H

#include "gtools.h"
#include <glm/mat4x4.hpp>

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class CCamera
{
public:
    struct Rect {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    CCamera(float height, float ratio, int windowHeight);
    void resize(double width, double height);
    Rect getRect() const;
    glm::mat4 getPMatrix() const;

private:
    /// Height in game units
    float height;
    /// RealWidth(px) / RealHeight(px)
    float ratio;
    /// RealHeight(px) / height(units)
    float coeff;

};

#endif // CCAMERA_H
