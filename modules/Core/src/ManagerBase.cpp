#include "ManagerBase.h"
#include "Entity.h"

namespace flappy {

void ManagerBase::postEvent(EventHandle&& eventHandle) {
    entity()->events()->post(eventHandle);
}

} // flappy
