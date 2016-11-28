#pragma once

#include <list>
#include <memory>

namespace flappy {

class Res: public std::enable_shared_from_this<Res> {
public:
    Res() = default;
    virtual ~Res() = default;
    Res(const Res&) = delete;
    Res& operator=(const Res&) = delete;
    Res(Res&&) = default;
    Res& operator=(Res&&) = default;

    virtual std::list<std::shared_ptr<Res>> dependencyList() const = 0;

    std::shared_ptr<Res> nextRes() {
        if (m_next != nullptr)
            return m_next->nextRes();
        else
            return shared_from_this();
    }
    void pushRes(std::shared_ptr<Res> res) {
        if (m_next != nullptr)
            nextRes()->pushRes(res);
        else
            m_next = res;
    }

private:
    std::shared_ptr<Res> m_next;
};


} // flappy
