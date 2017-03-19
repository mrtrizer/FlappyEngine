#include "Component.h"

#include <EventController.h>

#include "Entity.h"
#include "Manager.h"

namespace flappy
{

Component::Component():
    m_eventController(std::make_shared<EventController>())
{
    events()->subscribeIn([this](const AManager::OnManagerAdded& e) {
        m_managers.setById(e.id, e.pointer);
    });
    events()->subscribeIn([this](const AManager::OnManagerRemoved& e) {
        m_managers.setById(e.id, SafePtr<AManager>());
    });
    events()->subscribeIn([this](const OnUpdate& e) {
        update(e.dt);
    });
}

void Component::setParentEntity(SafePtr<Entity> entity)
{
    if (entity != m_entity) {
        if (m_entity) {
            deinitInternal();
        }
        m_entity = entity;
        if (entity) {
            initInternal();
        }
    }
}

void Component::initInternal()
{
    init();
}

void Component::deinitInternal()
{
    deinit();
}

} // flappy
