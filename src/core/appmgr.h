#pragma once

#include "manager.h"

namespace flappy {

class AppMgr: public Manager<AppMgr>
{
public:
    AppMgr(int argc = 1, char **argv = nullptr);

    std::vector<std::string> args() { return m_args; }

private:
    std::vector<std::string> m_args;
};

} // flappy
