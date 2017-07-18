#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class ManagerBase: public ComponentBase
{
public:

    using ComponentBase::ComponentBase;

    virtual TypeId<ComponentBase> componentId() = 0;

protected:


    void postEvent(EventHandle&& eventHandle);
};

} // flappy
