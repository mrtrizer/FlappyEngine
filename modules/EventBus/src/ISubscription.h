#pragma once

#include "EventHandle.h"

namespace flappy {

class ISubscription {
public:
    virtual ~ISubscription() = default;
    virtual void call(const EventHandle& e) = 0;
};

template <typename EventT>
class Subscription: public ISubscription {
public:
    Subscription(std::function<void(EventT)> func): m_func(func) {}

    void call(const EventHandle& e) override {
        m_func(*static_cast<EventT*>(e.eventPtr()));
    }

private:
    std::function<void(EventT)> m_func;
};

class SubscriptionAll: public ISubscription {
public:
    SubscriptionAll(std::function<void(const EventHandle&)> func): m_func(func) {}

    void call(const EventHandle& e) override {
        m_func(e);
    }

private:
    std::function<void(const EventHandle& e)> m_func;
};

} // flappy
