#include "DesktopThread.h"

#include <unistd.h>

#include <ThreadManager.h>

namespace flappy {

DeltaTime DesktopThread::calcTimeDelta() {
    using namespace std::chrono;
    auto newTime = steady_clock::now();
    auto diff = newTime - m_lastTime;
    DeltaTime timeDelta = diff.count(); // delta in seconds
    m_lastTime = newTime;
    return timeDelta;
}

int DesktopThread::run(std::function<void(SafePtr<Entity>)> initCallback) {
    auto entity = std::make_shared<Entity>();

    auto threadManager = entity->component<ThreadManager>();

    initCallback(entity);

    // Simplest draft implementation of the loop
    while(threadManager->active())
    {
        // Wait before next loop.
        usleep( 1000000 / updateRate() );

        auto updateEvent = ComponentBase::UpdateEvent(calcTimeDelta());
        entity->events()->post(updateEvent);
    }

    return 0;
}

} // flappy
