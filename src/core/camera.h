#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "component.h"

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

    Camera(float m_height = 100, float m_ratio = 1);
    Rect rect() const;
    glm::mat4 pMatrix();
    glm::vec3 screenToScene(const glm::vec3& pos) const;
    void init();

private:
    /// Height in game units
    float m_height;
    /// RealWidth(px) / RealHeight(px)
    float m_ratio;

};
