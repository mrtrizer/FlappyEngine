#include "flappyapp.h"

#include "entitymgr.h"
#include "scenemgr.h"
#include "screenmgr.h"
#include "viewmgr.h"
#include "inputmgr.h"
#include "gamemgr.h"
#include "myflappyapp.h"

using namespace std;

void FlappyApp::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - m_lastTime).count() / 1000.0f;
    m_lastTime = newTime;

    m_inputMgr->update(dt);
    m_entityMgr->update(dt);
    m_viewMgr->update(dt);
}

void FlappyApp::configure() {
    m_configured = true;
    m_lastTime = chrono::steady_clock::now();
    m_inputMgr = std::make_shared<InputMgr>();
    m_entityMgr = std::make_shared<EntityMgr>();
    m_sceneMgr = std::make_shared<SceneMgr>();
    m_screenMgr = std::make_shared<ScreenMgr>();
    m_gameMgr = std::make_shared<MyGameMgr>();
    m_gameMgr->init();
}
