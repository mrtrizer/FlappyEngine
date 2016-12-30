#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

#include <ClassId.h>
#include <FuncSignature.h>

#include "ISubscription.h"

namespace flappy {

class EventBus {
public:
    EventBus(): m_subscriptions(ClassCounter<EventBus>::count())
    {}

    void addChild(std::shared_ptr<EventBus> eventBus) {
        m_children.push_back(eventBus);
    }

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    template<typename FuncT>
    std::shared_ptr<ISubscription> subscribe(FuncT func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        const unsigned id = ClassId<EventBus, EventT>::id();
        auto subscription = std::make_shared<Subscription<EventT>>(func);
        m_subscriptions[id].push_back(subscription);
        return subscription;
    }

    /// Emit event to all listeners
    template<typename EventT>
    void post(EventT event) {
        const unsigned id = ClassId<EventBus, EventT>::id();
        auto& subscriptions = m_subscriptions[id];
        for (auto subscriptionIter = subscriptions.begin(); subscriptionIter != subscriptions.end(); subscriptionIter++) {
            if (subscriptionIter->use_count() == 1)
                subscriptionIter = subscriptions.erase(subscriptionIter);
            else
                std::static_pointer_cast<Subscription<EventT>>(*subscriptionIter)->call(event);
        }
        for (auto childIter = m_children.begin(); childIter != m_children.end(); childIter++) {
            if (childIter->use_count() == 1)
                childIter = m_children.erase(childIter);
            else
                (*childIter)->post<EventT>(std::forward<EventT>(event));
        }
    }

private:
    std::vector<std::list<std::shared_ptr<ISubscription>>> m_subscriptions;
    std::vector<std::shared_ptr<EventBus>> m_children;
};

} // flappy
