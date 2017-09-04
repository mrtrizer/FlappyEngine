#pragma once

#include <memory>

#include <ClassId.h>

#include "ManagerBase.h"

namespace flappy
{

template <typename DerivedT>
class Manager: public ManagerBase {
public:
    using ManagerBase::ManagerBase;

    TypeId<ComponentBase> componentId() override {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

    static TypeId<ComponentBase> id() {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

private:

    template <typename EventT>
    EventHandle createComponentEvent() {
        auto event = EventT();
        event.id = componentId();
        event.pointer = selfPointer<Manager<DerivedT>>();
        return EventHandle(event);
    }

    void initInternal() override final
    {
        LOGI("Init %s", typeName<DerivedT>().c_str());

        events()->post(InitEvent());
        init();
        // And ofc send message informing children about the manager was initialized
        postEvent(createComponentEvent<ManagerAddedEvent>());
    }

    void deinitInternal() override final
    {
        LOGI("Deinit %s", typeName<DerivedT>().c_str());

        // Send remove event first.
        // To allow components access manager before denitialization.
        postEvent(createComponentEvent<ManagerRemovedEvent>());

        // If we have parent manager of same type, notify dependant components
        if (isManagerRegistered(id()))
            postEvent(createComponentEvent<ManagerAddedEvent>());

        events()->post(DeinitEvent());
        deinit();
    }

    void addedToEntity() override final
    {
        if (isInitialized())
            postEvent(createComponentEvent<ManagerAddedEvent>());
    }

    void removedFromEntity() override final
    {
        if (isInitialized())
            postEvent(createComponentEvent<ManagerRemovedEvent>());
    }
};

} // flappy
