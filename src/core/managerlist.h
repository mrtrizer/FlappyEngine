#pragma once

#include <chrono>
#include <memory>
#include <vector>

namespace flappy {

class AbstractManager;

class ManagerList : public std::enable_shared_from_this<ManagerList> {
public:
    virtual ~ManagerList(){}
    void update();
    void init();

    template <typename Mgr> inline
    Mgr* MGR() { //TODO: Can I return refrence to shared_ptr?
        AbstractManager* mgr = nullptr;
        if (m_mgrList.size() >= Mgr::counter.id() + 1) //TODO: Can I avoid this checking?
            mgr = m_mgrList[Mgr::counter.id()].get();
        if (mgr) { //if found
            return reinterpret_cast<Mgr*>(mgr);
        } else { //search in parent
            if (auto parent = m_parent.lock())
                return parent->MGR<Mgr>();
            else
                return nullptr;
        }
    }

    template <typename Mgr, typename...T> inline
    void createMgr(T&&...args) {
        addMgr<Mgr>(std::make_shared<Mgr>(std::forward<T>(args)...));
        MGR<Mgr>()->init();
    }

    template <typename IMgr, typename Mgr, typename ... T> inline
    void overrideMgr(T&&...args) {
        setMgrAtPos(IMgr::counter.id(), std::make_shared<Mgr>(std::forward<T>(args)...));
    }

    template <typename Mgr> inline
    void addMgr(std::shared_ptr<Mgr> mgr) {
        setMgrAtPos(Mgr::counter.id(), mgr);
    }

    void setParent(std::weak_ptr<ManagerList> parent) {
        m_parent = parent;
    }

private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::shared_ptr<AbstractManager>> m_mgrList;
    std::weak_ptr<ManagerList> m_parent;

    void setMgrAtPos(unsigned int pos, std::shared_ptr<AbstractManager> mgr);
};

} // flappy
