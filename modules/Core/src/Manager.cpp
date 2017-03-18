#include "Manager.h"

#include "Entity.h"

namespace flappy {

void Manager::initInternal()
{
    // Initilize manager first. To be prepeared when components try access it.
    init();

    // Send OnManagerAdded event
    auto onManagerAddedEvent = OnManagerAdded();
    onManagerAddedEvent.id = ClassId<Component, Manager>::id();
    onManagerAddedEvent.pointer = selfPointer<Manager>();
    entity()->events()->post(std::move(onManagerAddedEvent));
}

void Manager::deinitInternal()
{
    // Send OnManagerRemoved event first.
    // To allow components access manager before denitialization.
    auto onManagerRemovedEvent = OnManagerRemoved();
    onManagerRemovedEvent.id = ClassId<Component, Manager>::id();
    onManagerRemovedEvent.pointer = selfPointer<Manager>();
    entity()->events()->post(std::move(onManagerRemovedEvent));

    // Deinitialize manager
    deinit();
}

} // flappy
