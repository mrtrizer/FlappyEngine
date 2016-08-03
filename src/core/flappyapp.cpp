#include "flappyapp.h"

#include "entitymgr.h"
#include "scenemgr.h"
#include "screenmgr.h"
#include "viewmgr.h"
#include "inputmgr.h"
#include "gamemgr.h"
#include "mygamemgr.h"

namespace flappy {

using namespace std;
using namespace game;

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
    m_inputMgr = make_shared<InputMgr>();
    m_entityMgr = make_shared<EntityMgr>();
    m_sceneMgr = make_shared<SceneMgr>();
    m_screenMgr = make_shared<ScreenMgr>();
    m_gameMgr = make_shared<MyGameMgr>();
    m_gameMgr->init();
}

} // flappy
