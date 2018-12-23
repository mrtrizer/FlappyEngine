#pragma once

#include <list>
#include <typeinfo>
#include <map>
#include <functional>

#include <Entity.hpp>

#include "CameraComponent.h"

namespace flappy
{

class [[manager]] SceneManager
{
public:

    void setMainCamera(const Handle<CameraComponent>& camera);

    Handle<CameraComponent> mainCamera() const { return m_camera; }

private:
    Handle<CameraComponent> m_camera;
};

} // flappy
