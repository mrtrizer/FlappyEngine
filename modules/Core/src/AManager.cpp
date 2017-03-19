#include "AManager.h"
#include "Entity.h"

namespace flappy {

void AManager::postEvent(EventHandle&& eventHandle) {
    entity()->events()->post(eventHandle);
}

} // flappy
