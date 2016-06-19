#ifndef CCAMERA_H
#define CCAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <core/screenmanager.h>
#include <core/component.h>

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class CCamera : public Component
{
public:
    struct Rect {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    CCamera(const entityx::Entity& e, float height = 100, float ratio = 1);
    Rect getRect() const;
    glm::mat4 getPMatrix();
    glm::vec3 screenToScene(glm::vec3 pos) const;

private:
    /// Height in game units
    float height;
    /// RealWidth(px) / RealHeight(px)
    float ratio;

};

#endif // CCAMERA_H