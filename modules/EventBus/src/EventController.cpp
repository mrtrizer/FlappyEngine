#include "EventController.h"

#include <Tools.h>

namespace flappy {

EventController::EventController():
    m_eventBus(std::make_shared<EventBus>())
{

}

void EventController::unsubscribe(std::shared_ptr<ISubscription> subscription) {
    m_subscriptionVector.remove(subscription);
}

} // flappy
