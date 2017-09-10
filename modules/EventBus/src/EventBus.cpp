#include "EventBus.h"

namespace flappy {

void EventBus::postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription>>& subscriptions)
{
    for (auto subscriptionIter = subscriptions.begin(); subscriptionIter != subscriptions.end(); ) {
        if (subscriptionIter->expired())
            subscriptionIter = subscriptions.erase(subscriptionIter);
        else {
            subscriptionIter->lock()->call(event);
            subscriptionIter++;
        }
    }
}

std::shared_ptr<ISubscription> EventBus::subscribeAll(std::function<void(const EventHandle& event)> handler) {
    auto subscription = std::make_shared<SubscriptionAll>(handler);
    m_abstractSubscriptions.push_back(subscription);
    return subscription;
}

void EventBus::post(const EventHandle& event) {
    auto id = event.id();
    postInList(event, m_inSubscriptions.getById(id));
    postInList(event, m_abstractSubscriptions);
}

} // flappy
