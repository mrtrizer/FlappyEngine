#include "IManager.h"
#include "Entity.h"

namespace flappy {

void IManager::postEvent(EventHandle&& eventHandle) {
    entity()->events()->post(eventHandle);
}

} // flappy
