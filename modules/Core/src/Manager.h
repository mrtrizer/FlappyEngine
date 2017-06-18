#pragma once

#include <memory>

#include <ClassId.h>

#include "IManager.h"

namespace flappy
{

template <typename DerivedT>
class Manager: public IManager {
public:
    using ClassIdList = std::list<unsigned>;

    using IManager::IManager;

    unsigned managerId() override final {
        return ClassId<Component, DerivedT>::id();
    }

    static unsigned id() {
        return ClassId<Component, DerivedT>::id();
    }

private:

    template <typename EventT>
    EventHandle createManagerEvent() {
        auto event = EventT();
        event.id = managerId();
        event.pointer = selfPointer<Manager<DerivedT>>();
        return EventHandle(event);
    }

    void initInternal() override final
    {
        LOGI("%s::initInternal()", typeName<DerivedT>().c_str());

        init();
        // And ofc send message informing children about the manager was initialized
        postEvent(createManagerEvent<OnManagerAdded>());
    }

    void deinitInternal() override final
    {
        LOGI("%s::deinitInternal()", typeName<DerivedT>().c_str());

        // Send remove event first.
        // To allow components access manager before denitialization.
        postEvent(createManagerEvent<OnManagerRemoved>());

        // If we have parent manager of same type, notify dependant components
        if (isManagerRegistered(id()))
            postEvent(createManagerEvent<OnManagerAdded>());

        deinit();
    }

    void addedToEntity() override final
    {
        if (isInitialized())
            postEvent(createManagerEvent<OnManagerAdded>());
    }

    void removedFromEntity() override final
    {
        if (isInitialized())
            postEvent(createManagerEvent<OnManagerRemoved>());
    }
};

} // flappy
