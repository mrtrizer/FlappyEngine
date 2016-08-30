#pragma once

#include <vector>

#include <core/managerlist.h>

namespace flappy {

class FlappyApp: public ManagerList {
public:
    FlappyApp(int argc = 1, const char **argv = nullptr);
    FlappyApp(const FlappyApp&) = delete;
    void operator=(const FlappyApp&) = delete;

    std::vector<const char*> args() { return m_args; }

private:
    std::vector<const char*> m_args;

};

} // flappy
