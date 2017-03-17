#include "Component.h"

#include <EventController.h>

#include "Entity.h"
#include "Manager.h"

namespace flappy
{

Component::Component():
    m_eventController(std::make_shared<EventController>())
{
    events()->subscribe([this](const Manager::OnManagerAdded& e) {
        m_managers.setById(e.id, e.pointer);
    });
    events()->subscribe([this](const Manager::OnManagerRemoved& e) {
        m_managers.setById(e.id, SafePtr<Component>());
    });
    events()->subscribe([this](const OnUpdate& e) {
        update(e.dt);
    });
}

void Component::setParentEntity(SafePtr<Entity> entity)
{
    if (entity != m_entity) {
        if (m_entity) {
            deinitInternal();
            deinit();
        }
        m_entity = entity;
        if (entity) {
            initInternal();
            init();
        }
    }
}

} // flappy
