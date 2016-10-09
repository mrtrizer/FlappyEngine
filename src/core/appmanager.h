#pragma once

#include "manager.h"

namespace flappy {

class AppManager: public Manager<>
{
public:
    AppManager(int argc = 1, const char* const* argv = nullptr);

    std::vector<std::string> args() { return m_args; }

private:
    std::vector<std::string> m_args;
};

} // flappy
