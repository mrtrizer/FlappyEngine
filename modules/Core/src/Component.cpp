#include "Component.h"

#include <EventController.h>

#include "Entity.h"

namespace flappy {

Component::Component():
    m_eventController(std::make_shared<EventController>())
{
    events()->eventBus()->subscribeIn([this](OnManagerAdded e) {
        m_managers.setById(e.id, e.pointer);
    });
    events()->eventBus()->subscribeIn([this](OnManagerRemoved e) {
        m_managers.setById(e.id, nullptr);
    });
}

} // flappy
