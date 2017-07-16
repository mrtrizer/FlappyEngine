#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <Tools.h>
#include <TextureRes.h>
#include <Component.h>

namespace flappy {

/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by View
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class CameraComponent : public Component<CameraComponent>
{
public:
    void setSize(glm::vec2 size)
    {
        m_height = size.y;
    }
    Tools::Rect rect() const;
    glm::mat4 pMatrix();
    glm::vec3 screenToScene(const glm::vec2 &pos) const;
    void init();

private:
    /// Height in game units
    float m_height = 100;
};

} // flappy
