#pragma once

#include <functional>

#include <SafePtr.h>
#include <Entity.h>
#include <TypeMap.h>

namespace flappy {

class Thread {
public:
    virtual int run() = 0;

    template <typename ManagerT>
    void setManager(SafePtr<ManagerT> manager) {
        m_managers.set<ManagerT>(manager);
    }

    template <typename ManagerT>
    SafePtr<ManagerBase> getManager() {
        return m_managers.get<ManagerT>();
    }

private:
    TypeMap<ComponentBase, SafePtr<ManagerBase>> m_managers;
};

} //flappy
