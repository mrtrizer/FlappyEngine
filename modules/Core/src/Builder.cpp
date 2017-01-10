#include "Builder.h"

namespace flappy {

Builder::Builder(std::weak_ptr<ManagerList> managerList)
{
    m_managerList = managerList;
}

} // flappy
