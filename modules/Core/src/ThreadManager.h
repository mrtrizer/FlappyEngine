#pragma once

#include "Manager.h"

namespace flappy {

class ThreadManager: public Manager<ThreadManager> {
public:
    bool active() { return m_active; }
    void quit() { m_active = false; }
private:
    bool m_active = true;
};

} // flappy
