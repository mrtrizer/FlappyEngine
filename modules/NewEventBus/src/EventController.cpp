#include "EventController.h"

#include <Utility.hpp>

namespace flappy {

EventController::EventController():
    m_eventBus(std::make_shared<EventBus>())
{

}

std::weak_ptr<ISubscription> EventController::subscribeAll(std::function<void(const EventHandle& event)> handler)
{
    auto subscription = m_eventBus->subscribeAll(handler);
    m_subscriptionVector.push_back(subscription);
    return subscription;
}

void EventController::unsubscribe(std::weak_ptr<ISubscription> subscription) {
    m_subscriptionVector.remove_if([subscription](std::shared_ptr<ISubscription>& item) {
        // https://stackoverflow.com/questions/12301916/equality-compare-stdweak-ptr
        return !subscription.owner_before(item) && !item.owner_before(subscription);
    });
}

} // flappy
