#pragma once

#include <memory>

#include "AManager.h"

namespace flappy
{

template <typename DerivedT>
class Manager: public AManager {
public:
    int managerId() override final {
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
        // Initilize manager first. To be prepeared when components try access it.
        init();

        // Send OnManagerAdded event
        postEvent(createManagerEvent<OnManagerAdded>());
    }

    void deinitInternal() override final
    {
        // Send OnManagerRemoved event first.
        // To allow components access manager before denitialization.
        postEvent(createManagerEvent<OnManagerRemoved>());

        // Deinitialize manager
        deinit();
    }
};

} // flappy
