#include "EventController.h"

namespace flappy {

EventController::EventController(std::shared_ptr<EventBus> parentEventBus):
    m_eventBus(std::make_shared<EventBus>())
{
    if (parentEventBus != nullptr)
        parentEventBus->addChild(m_eventBus);
}

} // flappy
