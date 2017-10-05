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

    TypeId<ComponentBase> componentId() const final {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

    static TypeId<ComponentBase> id() {
        return GetTypeId<ComponentBase, DerivedT>::value();
    }

protected:
    SafePtr<DerivedT> selfPointer() { return std::static_pointer_cast<DerivedT>(shared_from_this()); }
    const SafePtr<DerivedT> selfPointer() const { return std::static_pointer_cast<DerivedT>(shared_from_this()); }

private:

    template <typename EventT>
    EventHandle createComponentEvent() {
        auto event = EventT();
        event.id = componentId();
        event.pointer = selfPointer();
        return EventHandle(event);
    }

    void initInternal() override final
    {
        LOGI("Init %s", typeName<DerivedT>().c_str());

        events()->post(InitEvent());
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
        if (isManagerInitialized(id()))
            postEvent(createComponentEvent<ManagerAddedEvent>());

        events()->post(DeinitEvent());
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
