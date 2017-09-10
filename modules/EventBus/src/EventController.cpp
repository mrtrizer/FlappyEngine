#include "EventController.h"

#include <Tools.h>

namespace flappy {

EventController::EventController():
    m_eventBus(std::make_shared<EventBus>())
{

}

void EventController::unsubscribe(SafePtr<ISubscription> subscription) {
    m_subscriptionVector.remove_if([subscription](std::shared_ptr<ISubscription>& item) {return subscription == item; });
}

} // flappy
