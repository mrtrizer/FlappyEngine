#include "EventBus.h"

namespace flappy {

FlowStatus EventBus::postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription>>& subscriptions)
{
    FlowStatus flowStatus = FlowStatus::CONTINUE;
    for (auto subscriptionIter = subscriptions.begin(); subscriptionIter != subscriptions.end(); ) {
        if (subscriptionIter->expired())
            subscriptionIter = subscriptions.erase(subscriptionIter);
        else {
            if (subscriptionIter->lock()->call(event) == FlowStatus::BREAK)
                flowStatus = FlowStatus::BREAK;
            subscriptionIter++;
        }
    }
    return flowStatus;
}

} // flappy
