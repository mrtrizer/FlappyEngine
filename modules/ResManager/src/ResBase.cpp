#include "ResBase.h"

#include <list>
#include <memory>

namespace flappy {

std::shared_ptr<ResBase> ResBase::nextResBase() {
    if (m_next != nullptr)
        return m_next;
    else
        return shared_from_this();
}

std::shared_ptr<ResBase> ResBase::lastResBase() {
    if (m_next != nullptr)
        return m_next->lastResBase();
    else
        return shared_from_this();
}


bool ResBase::resUpdated() {
    return (m_next != nullptr) && (m_next.get() != this);
}

void ResBase::pushRes(std::shared_ptr<ResBase> res) {
    if (m_next != nullptr)
        lastResBase()->pushRes(res);
    else
        m_next = res;
}

} // flappy
