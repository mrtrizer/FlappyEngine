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

Component::~Component() {

}

void Component::subscribeEvents() {
    events()->subscribeIn([this](const OnManagerAdded& e) {
        m_managers.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesReady())
            tryInit();
    });
    events()->subscribeIn([this](const OnManagerRemoved& e) {
        m_managers.setById(e.id, SafePtr<IManager>());
        if (isInitialized() && !allDependenciesReady())
            tryDeinit();
    });
}

void Component::setParentEntity(SafePtr<Entity> entity)
{
    if (entity != m_entity) {
        // Deinit if entity was set before
        if (m_entity) {
            removedFromEntityInternal();
        }
        m_entity = entity;
        if (entity) {
            addedToEntityInternal();
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

void Component::tryInit() {
    // We have to set the flag before initialization call to avoid recursive calls
    m_initializedFlag = true;
    try {
        initInternal();
        m_updateSubscription = events()->subscribeIn([this](const OnUpdate& e) {
            update(e.dt);
        });
    } catch (std::exception& e) {
        LOGE("Initialization error: %s", e.what());
        m_initializedFlag = false;
    }
}

void Component::tryDeinit() {
    m_initializedFlag = false;
    try {
        // Send remove event first.
        // To allow components access manager before denitialization.
        deinitInternal();
        events()->unsubscribe(m_updateSubscription.lock());
    } catch (std::exception& e) {
        LOGE("Deinitialization error: %s", e.what());
        m_initializedFlag = true;
    }
}

void Component::addedToEntityInternal() {
    if (!isInitialized() && allDependenciesReady())
        tryInit();
    addedToEntity();
}

void Component::removedFromEntityInternal()
{
    if (isInitialized())
        tryDeinit();
    removedFromEntity();
}

} // flappy
