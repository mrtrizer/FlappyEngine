#include "Component.h"

#include <EventController.h>
#include <Tools.h>

#include "Entity.h"
#include "Manager.h"

namespace flappy
{

Component::Component():
    m_eventController(std::make_shared<EventController>()),
    m_dependenceClassIdList()
{
    subscribeEvents();
}

Component::Component(ClassIdList dependenceClassIdList)
    : m_eventController(std::make_shared<EventController>()),
      m_dependenceClassIdList(dependenceClassIdList)
{
    subscribeEvents();
}

void Component::subscribeEvents() {
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
        // Deinit if entity was set before
        if (m_entity) {
            deinitExternal();
        }
        m_entity = entity;
        if (entity) {
            initExternal();
        }
    }
}

void Component::initInternal() {
    init();
}

void Component::deinitInternal() {
    deinit();
}

bool Component::allDependenciesReady() {
    for (unsigned dependenceTypeId : m_dependenceClassIdList)
        if (!isManagerRegistered(dependenceTypeId))
            return false;
    return true;
}

bool Component::checkDependencies() {
    // Init if all managers registered
    if (allDependenciesReady()) {

        try {
            initInternal();
        } catch (std::exception& e) {
            LOGE("Initialization error: %s", e.what());
            return false;
        }

        // Subscribe to remove manager events
        m_managerRemovedSubscription = events()->subscribeIn([this] (OnManagerRemoved) {

            for (unsigned dependenceTypeId : m_dependenceClassIdList)
                if (!isManagerRegistered(dependenceTypeId)) {
                    deinitInternal();
                    // Unsubscribe not to be deinitialized twice
                    events()->unsubscribe(m_managerRemovedSubscription.lock());

                    break;
                }
        });
        return true;
    } else {
        return false;
    }
}

void Component::initExternal()
{
    m_managerAddedSubscription = events()->subscribeIn([this] (OnManagerAdded e) {

        // We also get our own event if we will send it.
        // This check need only for Managers. May be I'll find better solution later.
        // I don't like very much that component indirectly depends of Manager because
        // of this code.
        // Without this check we would recursively send and get this event back.
        if (e.pointer == selfPointer<Component>())
            return;
        if (checkDependencies())
            events()->unsubscribe(m_managerAddedSubscription.lock());
    });
    if (checkDependencies())
        events()->unsubscribe(m_managerAddedSubscription.lock());


}

void Component::deinitExternal()
{
    // check, was deinit called before
    if (!isInitialized())
        return;

    // Send remove event first.
    // To allow components access manager before denitialization.
    deinitInternal();

    events()->unsubscribe(m_managerRemovedSubscription.lock());
}

} // flappy
