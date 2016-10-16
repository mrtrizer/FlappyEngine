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

void ManagerList::setManagerAtPos(unsigned pos, shared_ptr<BaseManager> manager) {
    if (m_managerList.size() <= pos)
        m_managerList.resize(pos + 1);
    m_managerList[pos] = manager;
    try {
        manager->setManagerList(shared_from_this());
    } catch (std::bad_weak_ptr) {
        LOGE("ManagerList was created out of std::shared_ptr.");
        throw;
    }
}

void ManagerList::removeManagerAtPos(unsigned pos) {
    bool hasDependent = false;
    for (auto& manager: m_managerList) {
        if (manager == nullptr)
            continue;
        if (!manager->checkSatisfied([pos](unsigned id){return id != pos;})) {
            hasDependent = true;
            break;
        }
    }
    if (hasDependent)
        return ERROR_MSG(VOID_VALUE, "Can't remove manager. It has dependent");

    Tools::resizeAndGet(m_managerList, pos) = nullptr;
}

void ManagerList::init() {
    m_lastTime = chrono::steady_clock::now();
}

} // flappy
