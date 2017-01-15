#include "Component.h"

#include <EventController.h>

#include "Entity.h"

namespace flappy {

Component::Component():
    m_eventController(std::make_shared<EventController>())
{}

std::weak_ptr<ManagerList> Component::managerList() {
    return entity().lock()->managerList();
}

} // flappy
