#pragma once

#include <Manager.h>

namespace flappy {

class GlutManager : public Manager<GlutManager>
{
protected:
    void init() override;
    int startMainLoop();

private:
    int m_glutWindowId = 0;

    bool initGlut(std::vector<std::string> args);
    int initWindow(std::string name, int width, int height);
};

} // flappy
