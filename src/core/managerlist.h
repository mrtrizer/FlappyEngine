#pragma once

#include <chrono>
#include <memory>
#include <vector>

namespace flappy {

class AbstractManager;

class ManagerList : public std::enable_shared_from_this<ManagerList> {
public:
    virtual ~ManagerList() = default;
    void update();
    void init();

    // TODO: What about returing of pointer or reference?
    template <typename ManagerT> inline
    std::shared_ptr<ManagerT> MGR() {
        using namespace std;
        shared_ptr<AbstractManager> manager = nullptr;
        if (m_managerList.size() > ManagerT::counter.id())
            manager = m_managerList[ManagerT::counter.id()];
        if (manager) { //if found
            return static_pointer_cast<ManagerT>(manager);
        } else { //search in parent
            if (auto parent = m_parent.lock())
                return parent->MGR<ManagerT>();
            else
                return nullptr;
        }
    }

    template <typename ManagerT, typename...ArgsT> inline
    std::shared_ptr<ManagerT> create(ArgsT&&...args) {
        auto manager = std::make_shared<ManagerT>(std::forward<ArgsT>(args)...);
        add<ManagerT>(manager);
        manager->init();
        return manager;
    }

    template <typename DestManagerT, typename ManagerT, typename ... ArgsT> inline
    std::shared_ptr<ManagerT> override(ArgsT&&...args) {
        auto manager = std::make_shared<ManagerT>(std::forward<ArgsT>(args)...);
        setManagerAtPos(DestManagerT::counter.id(), manager);
        return manager;
    }

    template <typename ManagerT> inline
    void add(const std::shared_ptr<ManagerT>& manager) {
        setManagerAtPos(ManagerT::counter.id(), manager);
    }

    void setParent(const std::weak_ptr<ManagerList>& parent) {
        m_parent = parent;
    }

private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::shared_ptr<AbstractManager>> m_managerList;
    std::weak_ptr<ManagerList> m_parent;

    void setManagerAtPos(unsigned int pos, std::shared_ptr<AbstractManager> manager);
};

} // flappy
