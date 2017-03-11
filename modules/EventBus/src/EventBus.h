#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

#include <TypeMap.h>
#include <FuncSignature.h>

#include "ISubscription.h"

namespace flappy {


class EventBus {
public:
    EventBus()
    {}

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribeIn(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return subscribeInList(std::forward<FuncT>(func), m_inSubscriptions.get<EventT>());
    }

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribeOut(FuncT&& func) {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        return subscribeInList(std::forward<FuncT>(func), m_outSubscriptions.get<EventT>());
    }

    std::shared_ptr<ISubscription> subscribeInAll(std::function<void(const EventHandle& event)> handler) {
        auto subscription = std::make_shared<SubscriptionAll>(handler);
        m_abstractSubscriptions.push_back(subscription);
        return subscription;
    }

    void post(const EventHandle& event) {
        auto id = event.id();
        postInList(event, m_inSubscriptions.getById(id));
        postInList(event, m_abstractSubscriptions);
        postInList(event, m_outSubscriptions.getById(id));
    }

private:
    TypeMap<EventHandle, std::list<std::weak_ptr<ISubscription>>> m_inSubscriptions;
    std::list<std::weak_ptr<ISubscription>> m_abstractSubscriptions;
    TypeMap<EventHandle, std::list<std::weak_ptr<ISubscription>>> m_outSubscriptions;

    void postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription>>& subscriptions);

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    template<typename FuncT>
    std::shared_ptr<ISubscription> subscribeInList(FuncT&& func, std::list<std::weak_ptr<ISubscription>>& subscriptions)
    {
        using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
        auto subscription = std::make_shared<Subscription<EventT>>(std::forward<FuncT>(func));
        subscriptions.push_back(subscription);
        return subscription;
    }
};

} // flappy
