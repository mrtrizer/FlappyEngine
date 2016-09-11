#include "managerlist.h"

#include <core/manager.h>

namespace flappy {

using namespace std;

void ManagerList::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - m_lastTime).count() / 1000.0f;
    m_lastTime = newTime;

    for (auto mgr: m_mgrList)
        if (mgr != nullptr)
            mgr->update(dt);
}

void ManagerList::setMgrAtPos(unsigned int pos, shared_ptr<AbstractManager> mgr) {
    if (m_mgrList.size() <= pos)
        m_mgrList.resize(pos + 1);
    m_mgrList[pos] = mgr;
    mgr->setManagerList(shared_from_this());
}

void ManagerList::init() {
    m_lastTime = chrono::steady_clock::now();
}

} // flappy
