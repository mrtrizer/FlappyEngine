#include "Component.h"

#include <EventController.h>
#include <Tools.h>

#include "Entity.h"
#include "Manager.h"

namespace flappy
{

ComponentBase::ComponentBase():
    m_eventController(std::make_shared<EventController>()),
    m_dependenceClassIdList()
{
    subscribeEvents();
}

ComponentBase::ComponentBase(ClassIdList dependenceClassIdList)
    : m_eventController(std::make_shared<EventController>()),
      m_dependenceClassIdList(dependenceClassIdList)
{
    subscribeEvents();
}

ComponentBase::~ComponentBase() {

}

bool ComponentBase::isManagerRegistered(unsigned id) {
    return m_managers.getById(id) != nullptr;
}

bool ComponentBase::isComponentRegistered(unsigned id) {
    return entity()->componentById(id) != nullptr;
}

void ComponentBase::subscribeEvents() {
    events()->subscribeIn([this](const ManagerAddedEvent& e) {
        m_managers.setById(e.id, e.pointer);
        if (!isInitialized() && allDependenciesReady())
            tryInit();
    });
    events()->subscribeIn([this](const ManagerRemovedEvent& e) {
        m_managers.setById(e.id, SafePtr<ManagerBase>());
        if (isInitialized() && !allDependenciesReady())
            tryDeinit();
    });
}

void ComponentBase::setParentEntity(SafePtr<Entity> entity)
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

void ComponentBase::initInternal() {
    init();
}

void ComponentBase::deinitInternal() {
    deinit();
}

bool ComponentBase::allDependenciesReady() {
    for (unsigned dependenceTypeId : m_dependenceClassIdList)
        if (!isManagerRegistered(dependenceTypeId) && !isComponentRegistered(dependenceTypeId))
            return false;
    return true;
}

void ComponentBase::tryInit() {
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

void ComponentBase::tryDeinit() {
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

void ComponentBase::addedToEntityInternal() {
    if (!isInitialized() && allDependenciesReady())
        tryInit();
    addedToEntity();
}

void ComponentBase::removedFromEntityInternal()
{
    if (isInitialized())
        tryDeinit();
    removedFromEntity();
}

} // flappy
