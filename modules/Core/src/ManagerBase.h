#pragma once

#include "Component.h"
#include "EventHandle.h"

namespace flappy {

class ManagerBase: public ComponentBase
{
public:
    using ComponentBase::ComponentBase;

protected:
    void postEvent(EventHandle&& eventHandle);
};

} // flappy
