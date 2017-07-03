#pragma once

#include <Manager.h>
#include <AGLManager.h>

namespace flappy {

class Sdl2Manager : public AGLManager
{
public:
    Sdl2Manager();
    int startMainLoop() override;
protected:
    void init() override;

private:
    int m_glutWindowId = 0;

    bool initGlut(std::vector<std::string> args);
    int initWindow(std::string name, int width, int height);
};

} // flappy
