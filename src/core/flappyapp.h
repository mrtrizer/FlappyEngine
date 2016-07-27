#ifndef FLAPPYAPP_H
#define FLAPPYAPP_H

#include <chrono>
#include <core/entitymgr.h>
#include <core/scenemgr.h>
#include <core/viewmanager.h>
#include <core/inputmgr.h>

class FlappyApp {
public:
    explicit FlappyApp();
    void update();
    virtual void init() = 0;
    void configure();
    void setWorldView(std::shared_ptr<ViewManager> worldView) { m_viewManager = worldView; }

private:
    bool m_configured = false;
    std::chrono::steady_clock::time_point m_lastTime;
    std::shared_ptr<ViewManager> m_viewManager;
    std::shared_ptr<EntityMgr> m_entityMgr;
    std::shared_ptr<InputMgr> m_inputMgr;
    std::shared_ptr<SceneMgr> m_sceneMgr;
    std::shared_ptr<ScreenMgr> m_screenMgr;
};

#endif // FLAPPYAPP_H
