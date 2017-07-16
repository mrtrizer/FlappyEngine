#pragma once

#include <memory>

#include <ClassId.h>

#include "ManagerBase.h"

namespace flappy
{

template <typename DerivedT>
class Manager: public ManagerBase {
public:
    using ClassIdList = std::list<unsigned>;

    using ManagerBase::ManagerBase;

    unsigned componentId() override {
        return ClassId<ComponentBase, DerivedT>::id();
    }

    static unsigned id() {
        return ClassId<ComponentBase, DerivedT>::id();
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
        LOGI("%s::initInternal()", typeName<DerivedT>().c_str());

        init();
        // And ofc send message informing children about the manager was initialized
        postEvent(createComponentEvent<ManagerAddedEvent>());
    }

    void deinitInternal() override final
    {
        LOGI("%s::deinitInternal()", typeName<DerivedT>().c_str());

        // Send remove event first.
        // To allow components access manager before denitialization.
        postEvent(createComponentEvent<ManagerRemovedEvent>());

        // If we have parent manager of same type, notify dependant components
        if (isManagerRegistered(id()))
            postEvent(createComponentEvent<ManagerAddedEvent>());

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
