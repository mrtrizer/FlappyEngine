#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include "component.h"
#include <core/tools.h>

namespace flappy {

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class Camera : public Component, public std::enable_shared_from_this<Camera>
{
public:
    Camera(float m_height = 100);
    Tools::Rect rect() const;
    glm::mat4 pMatrix();
    glm::vec3 screenToScene(const glm::vec3& pos) const;
    void init();

private:
    /// Height in game units
    float m_height;

};

} // flappy
