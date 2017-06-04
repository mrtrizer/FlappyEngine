#pragma once

#include <Manager.h>

namespace flappy {

class GlutManager : public Manager<GlutManager>
{
public:
    int startMainLoop();
protected:
    void init() override;

private:
    int m_glutWindowId = 0;

    bool initGlut(std::vector<std::string> args);
    int initWindow(std::string name, int width, int height);
};

} // flappy
