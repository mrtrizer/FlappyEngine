#pragma once

namespace flappy {

class BasicLoopManager {
public:
    void stop() { m_active = false; }
    bool active() { return m_active; }
    
private:
    bool m_active = true;
};

} // flappy
