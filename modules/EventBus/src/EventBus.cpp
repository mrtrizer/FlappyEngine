#include "EventBus.h"

namespace flappy {

void EventBus::postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription>>& subscriptions)
{
    for (auto subscriptionIter = subscriptions.begin(); subscriptionIter != subscriptions.end(); subscriptionIter++) {
        if (subscriptionIter->expired())
            subscriptionIter = subscriptions.erase(subscriptionIter);
        else
            subscriptionIter->lock()->call(event);
    }
}

} // flappy
