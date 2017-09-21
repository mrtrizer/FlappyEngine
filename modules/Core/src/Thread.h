#pragma once

#include <functional>

#include <SafePtr.h>
#include <Entity.h>
#include <TypeMap.h>
#include <EventController.h>

namespace flappy {

class Thread {
    struct ManagerData {

        ManagerData(SafePtr<ManagerBase> manager, SafePtr<ISubscription> subscription)
            : m_manager(manager)
            , m_subscription(subscription)
        {}
        SafePtr<ManagerBase> m_manager;
        SafePtr<ISubscription> m_subscription;
    };

public:

    Thread()
        : m_eventController(std::make_shared<EventController>())
    {}
    virtual int run() = 0;

    template <typename ManagerT>
    void setManager(SafePtr<ManagerT> manager) {
        auto newSubscription = manager->events()->subscribeAll([this](const EventHandle& handle) {
            m_eventController->post(handle);
        });

        SafePtr<ManagerData> oldManagerData = m_managers.get<ManagerT>();
        if (oldManagerData != nullptr)
            m_managers.get<ManagerT>()->m_manager->events()->unsubscribe(oldManagerData->m_subscription);

        auto newManagerData = std::make_shared<ManagerData>(manager, newSubscription);
        m_managers.set<ManagerT>(newManagerData);
    }

    template <typename ManagerT>
    SafePtr<ManagerBase> getManager() {
        if (m_managers.get<ManagerT>() == nullptr)
            return SafePtr<ManagerT>();
        return m_managers.get<ManagerT>()->m_manager;
    }

    SafePtr<EventController> eventController() {
        return m_eventController;
    }

    std::shared_ptr<EventController> events() {
        return m_eventController;
    }

private:
    TypeMap<ComponentBase, std::shared_ptr<ManagerData>> m_managers;
    std::shared_ptr<EventController> m_eventController;
};

} //flappy
