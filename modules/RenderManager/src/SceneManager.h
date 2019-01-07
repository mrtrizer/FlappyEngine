#pragma once

#include <Entity.hpp>
#include <EventBus.h>
#include <IEvent.h>

#include "CameraComponent.h"

namespace flappy
{

class REFLECT SceneManager
{
public:
    struct CameraChangedEvent : IEvent {};
    
    void setMainCamera(Handle<CameraComponent> camera);
    Handle<CameraComponent> mainCamera() const { return m_camera; }
    EventBus& eventBus() { return m_eventBus; }

private:
    EventBus m_eventBus;
    Handle<CameraComponent> m_camera;
};

} // flappy
