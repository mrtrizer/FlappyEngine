#pragma once

#include <memory>

#include <Entity.h>
#include <ManagerList.h>

namespace flappy {

class Builder
{
public:
    Builder();
    virtual ~Builder() = default;
    Builder(const Builder&) = delete;
    Builder& operator=(const Builder&) & = delete;
    Builder(Builder&&) = delete;
    Builder& operator=(Builder&&) & = delete;

    void setManagerList(std::shared_ptr<ManagerList> managerList) const {
        m_managerList = managerList;
    }

    virtual void build(std::shared_ptr<Entity>) const = 0;

private:
    mutable std::shared_ptr<ManagerList> m_managerList;

protected:
    template <typename ManagerT>
    auto manager() const -> decltype(m_managerList->manager<ManagerT>()) {
        return m_managerList->manager<ManagerT>();
    }

    std::shared_ptr<ManagerList> managerList() const {
        return m_managerList;
    }
};

} // flappy
