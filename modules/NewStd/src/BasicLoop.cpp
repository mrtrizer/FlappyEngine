#include "BasicLoop.hpp"

#include <unistd.h>

#include <Hierarchy.hpp>

#include "BasicLoopManager.hpp"

namespace flappy {

BasicLoop::BasicLoop(int updateRate, Handle<Hierarchy> hierarchy)
    : m_hierarchy(hierarchy)
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

int BasicLoop::run() {

    auto loopManager = m_hierarchy->initManager<BasicLoopManager>();

    // Simplest draft implementation of the loop
    while(loopManager->active())
    {
        // Wait before next loop.
        usleep( 1000000 / updateRate() );

        m_hierarchy->manager<UpdateManager>()->update(calcTimeDelta());
    }

    // FIXME: Remove LoopManager
    //m_hierarchy->removeManager<LoopManager>();
    
    return 0;
}

} // flappy
