#ifndef GOBJCAMERA_H
#define GOBJCAMERA_H

#include "gobj.h"
#include "gtools.h"
#include "gobjpointer.h"

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class GObjCamera
{
public:
    struct Rect {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    GObjCamera(float height, float ratio, int windowHeight);
    void resize(double width, double height);
    Rect getRect() const;
    GTools::PMatrix getPMatrix() const;

private:
    /// Height in game units
    float height;
    /// RealWidth(px) / RealHeight(px)
    float ratio;
    /// RealHeight(px) / height(units)
    float coeff;

};

#endif // GOBJCAMERA_H
