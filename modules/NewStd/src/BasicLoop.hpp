#pragma once

#include <functional>
#include <chrono>

#include <Handle.hpp>
#include <UpdateManager.hpp>

namespace flappy {

class Hierarchy;

class BasicLoop {
public:
    BasicLoop(int updateRate, Handle<Hierarchy> hierarchy);
    void setUpdateRate(int updateRate) { m_updateRate = updateRate; }
    int updateRate() { return m_updateRate; }
    int run();

private:
    Handle<Hierarchy> m_hierarchy;
    int m_updateRate = 30;
    std::chrono::steady_clock::time_point m_lastTime;

    DeltaTime calcTimeDelta();
};

} // flappy
