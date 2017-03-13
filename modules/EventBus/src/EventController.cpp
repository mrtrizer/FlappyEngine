#include "EventController.h"

namespace flappy {

EventController::EventController():
    m_eventBus(std::make_shared<EventBus>())
{

}

} // flappy
