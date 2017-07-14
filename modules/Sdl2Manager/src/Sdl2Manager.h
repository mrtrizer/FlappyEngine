#pragma once

#include <Manager.h>
#include <AGLManager.h>

namespace flappy {

class Sdl2Manager : public AGLManager
{
public:
    Sdl2Manager();
    int startMainLoop() override;
    void setMaxFps(int fps);
    void setUpdateFreq(int updateFreq);

protected:
    void init() override;

private:
    int m_maxFps;
    int m_updateFreq;

    bool initSdl2(std::vector<std::string> args);
    int initWindow(std::string name, int width, int height);
    void cleanup();
};

} // flappy
