#include "eventsystem.h"

namespace flappy {

void EventSystem::exec() {
    std::lock_guard<std::mutex> lock(syncMutex);
    for (auto& subscriptions: m_subscriptions[std::this_thread::get_id()])
        for (auto& subscription: subscriptions)
            subscription->exec();
}

} // flappy
