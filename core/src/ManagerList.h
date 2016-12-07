#pragma once

#include <chrono>
#include <memory>
#include <vector>

#include "ClassId.h"
#include "Tools.h"

namespace flappy {

class BaseManager;

class ManagerList : public std::enable_shared_from_this<ManagerList> {
public:
    ManagerList(): m_managerList(ClassCounter<BaseManager>::count()) {}
    virtual ~ManagerList() = default;
    void update();
    void init();

    template <typename ManagerT>
    std::shared_ptr<ManagerT> MGR() {
        using namespace std;
        shared_ptr<BaseManager> manager = m_managerList[ClassId<BaseManager, ManagerT>::id()];
        if (manager) { //if found
            return static_pointer_cast<ManagerT>(manager);
        } else { //search in parent
            if (auto parent = m_parent.lock())
                return parent->MGR<ManagerT>();
            else
                return nullptr;
                //throw std::runtime_error("Manager is not initialized: " + typeName<ManagerT>());
        }
    }

    /// Create manager with initialization
    template <typename ManagerT, typename...ArgsT>
    std::shared_ptr<ManagerT> create(ArgsT&&...args) {
        auto manager = std::make_shared<ManagerT>(std::forward<ArgsT>(args)...);
        add<ManagerT>(manager);
        manager->init();
        return manager;
    }

    /// Override manager interface with initialization
    template <typename DestManagerT, typename ManagerT, typename ... ArgsT>
    std::shared_ptr<ManagerT> override(ArgsT&&...args) {
        auto manager = std::make_shared<ManagerT>(std::forward<ArgsT>(args)...);
        setManagerAtPos(ClassId<BaseManager, DestManagerT>::id(), manager);
        manager->init();
        return manager;
    }

    template <typename ManagerT>
    void remove() {
        removeManagerAtPos(ClassId<BaseManager, ManagerT>::id());
    }

    /// Add without initialization
    template <typename ManagerT>
    void add(const std::shared_ptr<ManagerT>& manager) {
        setManagerAtPos(ClassId<BaseManager, ManagerT>::id(), manager);
    }

    void setParent(const std::weak_ptr<ManagerList>& parent) {
        m_parent = parent;
    }

private:
    std::chrono::steady_clock::time_point m_lastTime;
    std::vector<std::shared_ptr<BaseManager>> m_managerList;
    std::weak_ptr<ManagerList> m_parent;

    void setManagerAtPos(unsigned pos, std::shared_ptr<BaseManager> manager);
    void removeManagerAtPos(unsigned pos);
};

} // flappy
