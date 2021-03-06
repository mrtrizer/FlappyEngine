#include "EventBus.h"

namespace flappy {

// FIXME: Check if EventBus was not destroyed in the middle of sending events
FORDEBUG(static bool interruptPost = false;)

EventBus::~EventBus() {
    FORDEBUG(interruptPost = true;)
}

void EventBus::postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription>>& subscriptions)
{
    interruptPost = false;
    for (auto subscriptionIter = subscriptions.begin(); subscriptionIter != subscriptions.end(); ) {
        if (subscriptionIter->expired())
            subscriptionIter = subscriptions.erase(subscriptionIter);
        else {
            subscriptionIter->lock()->call(event);
            
            DEBUG_ASSERT(!interruptPost);
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
    auto subscriptionsIter = m_inSubscriptions.find(id);
    if (subscriptionsIter != m_inSubscriptions.end())
        postInList(event, subscriptionsIter->second);
    postInList(event, m_abstractSubscriptions);
}

} // flappy

