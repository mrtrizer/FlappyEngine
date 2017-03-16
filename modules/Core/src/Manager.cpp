#include "Manager.h"

#include "Entity.h"

namespace flappy {

void Manager::initInternal()
{
    auto onManagerAddedEvent = OnManagerAdded();
    onManagerAddedEvent.id = ClassId<Component, Manager>::id();
    onManagerAddedEvent.pointer = selfPointer<Manager>();
    entity()->events()->post(std::move(onManagerAddedEvent));
}

void Manager::deinitInternal()
{
    auto onManagerRemovedEvent = OnManagerRemoved();
    onManagerRemovedEvent.id = ClassId<Component, Manager>::id();
    onManagerRemovedEvent.pointer = selfPointer<Manager>();
    entity()->events()->post(std::move(onManagerRemovedEvent));
}

} // flappy
