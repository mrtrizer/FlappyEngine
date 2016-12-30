#include "ManagerList.h"

#include <Manager.h>

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

void ManagerList::setManagerAtPos(unsigned pos, shared_ptr<BaseManager> manager) {
    m_managerList[pos] = manager;
    try {
        manager->setManagerList(shared_from_this());
    } catch (std::bad_weak_ptr) {
        LOGE("ManagerList was created out of std::shared_ptr.");
        throw;
    }
}

void ManagerList::removeManagerAtPos(unsigned pos) {

    m_managerList[pos] = nullptr;
}

void ManagerList::init() {
    m_lastTime = chrono::steady_clock::now();
}

} // flappy
