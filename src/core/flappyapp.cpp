#include "flappyapp.h"

#include "entitymgr.h"
#include "scenemgr.h"
#include "screenmgr.h"
#include "viewmgr.h"
#include "inputmgr.h"
#include "gamemgr.h"
#include "mygamemgr.h"
#include <core/manager.h>
#include <core/resourcemgr.h>

namespace flappy {

using namespace std;
using namespace game;

void FlappyApp::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - m_lastTime).count() / 1000.0f;
    m_lastTime = newTime;

    for (auto mgr: m_mgrList)
        mgr->update(dt);
}

void FlappyApp::setMgrAtPos(unsigned int pos, shared_ptr<IManager> mgr) {
    if (m_mgrList.size() <= pos)
        m_mgrList.resize(pos + 1);
    m_mgrList[pos] = mgr;
    mgr->setFlappyApp(shared_from_this());
}

void FlappyApp::init() {
    m_lastTime = chrono::steady_clock::now();
    for (auto mgr: m_mgrList)
        mgr->init();
}

} // flappy
