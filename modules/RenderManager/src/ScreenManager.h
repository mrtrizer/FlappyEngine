#pragma once

#include <memory>

#include <glm/vec2.hpp>

#include <EventBus.h>
#include <IEvent.h>
#include <ReflectionMarkers.hpp>

namespace flappy {

class REFLECT ScreenManager
{
public:
    struct ScreenResizedEvent : public IEvent {};
    
    const glm::uvec2& screenSize() const { return m_screenSize; }
    void resize(const glm::uvec2& size);
    EventBus& eventBus() { return m_eventBus; }
    
private:
    EventBus m_eventBus;
    glm::uvec2 m_screenSize;
};

} // flappy
