#pragma once

namespace flappy {

class ISubscription {
public:
    virtual ~ISubscription() = default;
    virtual void exec() = 0;
};

template <typename EventT>
class Subscription: public ISubscription {
public:
    Subscription(std::function<void(EventT)> func): m_func(func) {}

    void call(const EventT& e) {
        m_func(e);
    }

    void add(const EventT& e) {
        call(e);
    }

    void exec() override {
        for (auto& event: m_eventQueue)
            call(event);
        m_eventQueue.clear();
    }

private:
    std::function<void(EventT)> m_func;
    std::list<EventT> m_eventQueue;
};

} // flappy
