#ifndef CCAMERA_H
#define CCAMERA_H

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <core/screenmgr.h>
#include <core/entitymgr.h>

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:
    struct Rect {
        float x1;
        float y1;
        float x2;
        float y2;
    };

    Camera(float height = 100, float ratio = 1);
    Rect getRect() const;
    glm::mat4 getPMatrix();
    glm::vec3 screenToScene(glm::vec3 pos) const;
    void init();

private:
    /// Height in game units
    float height;
    /// RealWidth(px) / RealHeight(px)
    float ratio;

};

#endif // CCAMERA_H
