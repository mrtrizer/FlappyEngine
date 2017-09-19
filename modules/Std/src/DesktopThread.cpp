#include "DesktopThread.h"

#include <unistd.h>

#include <ThreadManager.h>

namespace flappy {

DesktopThread::DesktopThread(std::function<void(SafePtr<Entity>)> initCallback)
    : m_initCallback(initCallback)
{}

DeltaTime DesktopThread::calcTimeDelta() {
    using namespace std::chrono;
    auto newTime = steady_clock::now();
    auto diff = newTime - m_lastTime;
    DeltaTime timeDelta = diff.count(); // delta in seconds
    m_lastTime = newTime;
    return timeDelta;
}

int DesktopThread::run() {
    auto entity = std::make_shared<Entity>();

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
