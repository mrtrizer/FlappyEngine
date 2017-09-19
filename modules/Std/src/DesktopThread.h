#pragma once

#include <Thread.h>

namespace flappy {

class DesktopThread: public Thread {
public:
    DesktopThread(std::function<void(SafePtr<Entity>)> initCallback);
    void setUpdateRate(int updateRate) { m_updateRate = updateRate; }
    int updateRate() { return m_updateRate; }
    int run() final;
private:
    int m_updateRate = 30;
    std::chrono::steady_clock::time_point m_lastTime;
    std::function<void(SafePtr<Entity>)> m_initCallback;

    DeltaTime calcTimeDelta();
};

} // flappy
