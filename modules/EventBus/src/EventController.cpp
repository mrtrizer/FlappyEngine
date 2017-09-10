#include "EventController.h"

#include <Tools.h>

namespace flappy {

EventController::EventController():
    m_eventBus(std::make_shared<EventBus>())
{

}

SafePtr<ISubscription> EventController::subscribeAll(std::function<void(const EventHandle& event)> handler)
{
    auto subscription = m_eventBus->subscribeAll(handler);
    m_subscriptionVector.push_back(subscription);
    return subscription;
}

void EventController::unsubscribe(SafePtr<ISubscription> subscription) {
    m_subscriptionVector.remove_if([subscription](std::shared_ptr<ISubscription>& item) {return subscription == item; });
}

} // flappy
