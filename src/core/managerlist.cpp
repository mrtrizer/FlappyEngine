#include "managerlist.h"

#include <core/manager.h>

namespace flappy {

using namespace std;

void ManagerList::update() {
    auto newTime = chrono::steady_clock::now();
    TimeDelta dt = chrono::duration <float, milli> (newTime - m_lastTime).count() / 1000.0f;
    m_lastTime = newTime;

    for (auto manager: m_managerList)
        if (manager != nullptr)
            manager->update(dt);
}

void ManagerList::setManagerAtPos(unsigned int pos, shared_ptr<AbstractManager> manager) {
    if (m_managerList.size() <= pos)
        m_managerList.resize(pos + 1);
    m_managerList[pos] = manager;
    manager->setManagerList(shared_from_this());
}

void ManagerList::init() {
    m_lastTime = chrono::steady_clock::now();
}

} // flappy
