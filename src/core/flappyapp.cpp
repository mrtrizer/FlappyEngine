#include <core/inputmgr.h>

#include "flappyapp.h"

using namespace std;

FlappyApp::FlappyApp() {
}

void FlappyApp::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - m_lastTime).count() / 1000.0f;
    m_lastTime = newTime;

//    m_inputMgr->update(dt);
//    m_entityMgr->update(dt);
    InputMgr::getInst()->update(dt);
    EntityMgr::getInst()->update(dt);
    m_viewManager->update(dt);
}

void FlappyApp::configure() {
    init();
    m_configured = true;
    m_lastTime = chrono::steady_clock::now();
}
