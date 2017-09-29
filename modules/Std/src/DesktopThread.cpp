#include "DesktopThread.h"

#include <unistd.h>

#include <ThreadManager.h>
#include <Entity.h>

namespace flappy {

DesktopThread::DesktopThread(std::function<void(SafePtr<Entity>)> initCallback)
    : m_initCallback(initCallback)
{}

DeltaTime DesktopThread::calcTimeDelta() {
    using namespace std::chrono;
    auto newTime = steady_clock::now();
    auto diff = newTime - m_lastTime;
    long msCount = duration_cast<milliseconds>(diff).count();
    DeltaTime timeDelta = (float)msCount / 1000.0f; // delta in seconds
    m_lastTime = newTime;
    return timeDelta;
}

int DesktopThread::run(SafePtr<Entity> entity) {

    auto threadManager = entity->component<ThreadManager>();

    m_initCallback(entity);

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
