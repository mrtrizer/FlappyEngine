#pragma once

#include <Thread.h>

namespace flappy {

class DesktopThread: public Thread {
public:
    void setUpdateRate(int updateRate) { m_updateRate = updateRate; }
    int updateRate() { return m_updateRate; }
    int run(std::function<void(SafePtr<Entity>)> initCallback) final;
private:
    int m_updateRate = 30;
    std::chrono::steady_clock::time_point m_lastTime;

    DeltaTime calcTimeDelta();
};

} // flappy
