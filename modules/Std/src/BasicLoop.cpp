#include "BasicLoop.hpp"

#include <unistd.h>

#include <Hierarchy.hpp>

#include "BasicLoopManager.hpp"

namespace flappy {

BasicLoop::BasicLoop(int updateRate, Handle<BasicLoopManager> basicLoopManager)
    : m_basicLoopManager(basicLoopManager)
    , m_updateRate(updateRate)
    , m_lastTime(std::chrono::steady_clock::now())
{}

DeltaTime BasicLoop::calcTimeDelta() {
    using namespace std::chrono;
    auto newTime = steady_clock::now();
    auto diff = newTime - m_lastTime;
    m_lastTime = newTime;
    long msCount = duration_cast<milliseconds>(diff).count();
    DeltaTime timeDelta = (float)msCount / 1000.0f; // delta in seconds
    return timeDelta;
}

int BasicLoop::run(const std::function<void(DeltaTime dt)>& update) {

    // Simplest draft implementation of the loop
    while(m_basicLoopManager->active())
    {
        // Wait before next loop.
        usleep( 1000000 / updateRate() );

        update(calcTimeDelta());
    }
    
    return 0;
}

} // flappy
