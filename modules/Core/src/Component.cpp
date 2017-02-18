#include "Component.h"

#include <EventController.h>

#include "Entity.h"

namespace flappy {

Component::Component():
    m_eventController(std::make_shared<EventController>())
{}

} // flappy
