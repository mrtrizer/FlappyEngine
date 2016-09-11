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

    template <typename Manager> inline
    Manager* MGR() { //TODO: Can I return refrence to shared_ptr?
        AbstractManager* manager = nullptr;
        if (m_managerList.size() >= Manager::counter.id() + 1) //TODO: Can I avoid this checking?
            manager = m_managerList[Manager::counter.id()].get();
        if (manager) { //if found
            return reinterpret_cast<Manager*>(manager);
        } else { //search in parent
            if (auto parent = m_parent.lock())
                return parent->MGR<Manager>();
            else
                return nullptr;
        }
    }

    template <typename Manager, typename...T> inline
    void create(T&&...args) {
        add<Manager>(std::make_shared<Manager>(std::forward<T>(args)...));
        MGR<Manager>()->init();
    }

    template <typename IManager, typename Manager, typename ... T> inline
    void override(T&&...args) {
        setManagerAtPos(IManager::counter.id(), std::make_shared<Manager>(std::forward<T>(args)...));
    }

    template <typename Manager> inline
    void add(std::shared_ptr<Manager> manager) {
        setManagerAtPos(Manager::counter.id(), manager);
    }

    void setParent(std::weak_ptr<ManagerList> parent) {
        m_parent = parent;
    }

private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::shared_ptr<AbstractManager>> m_managerList;
    std::weak_ptr<ManagerList> m_parent;

    void setManagerAtPos(unsigned int pos, std::shared_ptr<AbstractManager> manager);
};

} // flappy
