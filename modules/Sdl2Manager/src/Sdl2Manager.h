#pragma once

#include <SDL2/SDL.h>

#include <Manager.h>
#include <AGLManager.h>

namespace flappy {

class Sdl2Manager : public AGLManager
{
public:
    Sdl2Manager();
    int startMainLoop() override;
    void setMaxFps(int fps) { m_maxFps = fps; }
    void setUpdateFreq(int updateFreq) { m_updateFreq = updateFreq; }

    struct Sdl2Event : IEvent {
        Sdl2Event(SDL_Event event): event(event) {}
        SDL_Event event;
    };

protected:
    void init() override;

private:
    int m_maxFps;
    int m_updateFreq;
    SDL_Window* m_mainWindow;
    SDL_GLContext m_mainContext;
    std::chrono::steady_clock::time_point m_lastTime;

    DeltaTime calcTimeDelta();
    void resizeWindow(int width, int height);
    bool setOpenGLAttributes();
    bool initSdl2(std::vector<std::string> args);
    void initWindow(std::string name, int width, int height);
    void printSdlGlAttributes();
    void cleanup();
};

} // flappy
