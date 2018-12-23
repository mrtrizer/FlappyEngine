#pragma once

#include <functional>
#include <chrono>

#include <Handle.hpp>
#include <UpdateManager.hpp>

namespace flappy {

class BasicLoopManager;

class BasicLoop {
public:
    BasicLoop(int updateRate, Handle<BasicLoopManager> basicLoopManager);
    void setUpdateRate(int updateRate) { m_updateRate = updateRate; }
    int updateRate() { return m_updateRate; }
    int run(const std::function<void(DeltaTime dt)>& update);

private:
    Handle<BasicLoopManager> m_basicLoopManager;
    int m_updateRate = 30;
    std::chrono::steady_clock::time_point m_lastTime;

    DeltaTime calcTimeDelta();
};

} // flappy
