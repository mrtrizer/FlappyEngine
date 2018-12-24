#pragma once

#include <thread>
#include <mutex>
#include <list>
#include <unordered_map>
#include <vector>
#include <functional>

#include <FuncSignature.h>
#include <Utility.hpp>

#include "ISubscription.h"

namespace flappy {

class EventBus {
public:
    EventBus() = default;
    virtual ~EventBus() = default;
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) & = delete;
    EventBus(EventBus&&) = delete;
    EventBus& operator=(EventBus&&) & = delete;

    template <typename FuncT>
    std::shared_ptr<ISubscription> subscribe(FuncT&& func);

    std::shared_ptr<ISubscription> subscribeAll(std::function<void(const EventHandle& event)> handler);

    void post(const EventHandle& event);

private:
    // FIXME: Could be replaced with multimap
    std::unordered_map<TypeId, std::list<std::weak_ptr<ISubscription>>> m_inSubscriptions;
    std::list<std::weak_ptr<ISubscription>> m_abstractSubscriptions;

    void postInList(const EventHandle& event, std::list<std::weak_ptr<ISubscription> > &subscriptions);

    template <typename FuncT, typename CompT>
    using IsSameResult = std::is_same<typename FuncSignature<FuncT>::result_type, CompT>;

    template <typename FuncT>
    std::shared_ptr<ISubscription> createSubscription(FuncT&& func);

    /// Add subscription to event
    /// @param func callback function `void (const EventT& e)`
    template<typename FuncT>
    std::shared_ptr<ISubscription> subscribeList(FuncT&& func, std::list<std::weak_ptr<ISubscription>>& subscriptions);

};

template <typename FuncT>
std::shared_ptr<ISubscription> EventBus::subscribe(FuncT&& func) {
    using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
    auto typeId = getTypeId<std::decay_t<EventT>>();
    auto subscriptionIter = m_inSubscriptions.find(typeId);
    if (subscriptionIter == m_inSubscriptions.end()) {
        auto result = m_inSubscriptions.emplace(typeId, std::list<std::weak_ptr<ISubscription>>());
        USER_ASSERT(result.second); // successfully added
        subscriptionIter = result.first;
    }
    return subscribeList(std::forward<FuncT>(func), subscriptionIter->second);
}

template <typename FuncT>
std::shared_ptr<ISubscription> EventBus::createSubscription(FuncT&& func) {
    using EventT = typename FuncSignature<FuncT>::template arg<0>::type;
    return std::make_shared<Subscription<EventT>>(std::forward<FuncT>(func));
}

/// Add subscription to event
/// @param func callback function `void (const EventT& e)`
template<typename FuncT>
std::shared_ptr<ISubscription> EventBus::subscribeList(FuncT&& func, std::list<std::weak_ptr<ISubscription>>& subscriptions)
{
    auto subscription = createSubscription(func);
    subscriptions.push_back(subscription);
    return subscription;
}

} // flappy
