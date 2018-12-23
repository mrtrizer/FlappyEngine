#pragma once

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <MathUtils.h>
#include <TextureRes.h>
#include <Handle.hpp>
#include <EnableSelfHandle.hpp>

namespace flappy {

class Entity;
class Hierarchy;
class SceneManager;
class ScreenManager;
class TransformComponent;
    
/// @brief Game camera. Contains screen parameters.
/// @details A camera can be set as active in GWordCtrl and used by Render
/// to get a perspective transformation matrix. Also camera
/// manages coordinates of GObjPointer object.
class [[component]] CameraComponent : public EnableSelfHandle<CameraComponent>
{
public:
    CameraComponent(Handle<Hierarchy> hierarchy);
    
    void setEntity(Handle<Entity> entity);

    void setSize(glm::vec2 size)
    {
        m_height = size.y;
    }

    MathUtils::Rect rect() const;
    glm::mat4 pMatrix();
    glm::vec3 screenToScene(const glm::vec2 &pos) const;
    void init();

private:
    /// Height in game units
    float m_height = 100;
    Handle<SceneManager> m_sceneManager;
    Handle<ScreenManager> m_screenManager;
    Handle<TransformComponent> m_transformComponent;
};

} // flappy
