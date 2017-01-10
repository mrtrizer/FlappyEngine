#pragma once

#include <memory>

#include <Entity.h>
#include <ManagerList.h>

namespace flappy {

class Builder
{
public:
    Builder(std::weak_ptr<ManagerList> managerList);
    virtual ~Builder() = default;
    Builder(const Builder&) = delete;
    Builder& operator=(const Builder&) & = delete;
    Builder(Builder&&) = delete;
    Builder& operator=(Builder&&) & = delete;

    void setManagerList(std::shared_ptr<ManagerList> managerList) {
        m_managerList = managerList;
    }

    virtual std::shared_ptr<Entity> build() const = 0;

private:
    std::weak_ptr<ManagerList> m_managerList;

protected:
    template <typename ManagerT>
    auto manager() const -> decltype(m_managerList.lock()->manager<ManagerT>()) {
        return m_managerList.lock()->manager<ManagerT>();
    }

    std::shared_ptr<ManagerList> managerList() const {
        return m_managerList.lock();
    }
};

} // flappy
