#pragma once

#include <SDL2/SDL.h>
#include <EventBus.h>
#include <ISubscription.h>
#include <Updatable.hpp>

#include <IGLManager.h>

namespace flappy {
    
class BasicLoopManager;

class REFLECT Sdl2Manager : public IGLManager
{
public:
    Sdl2Manager(Handle<Hierarchy> hierarchy);
    ~Sdl2Manager();
    void setMaxFps(int fps) { m_maxFps = fps; }
    EventBus& eventBus() { return m_eventBus; }
    void update(DeltaTime dt);

    struct Sdl2Event : IEvent {
        Sdl2Event(SDL_Event event): event(event) {}
        SDL_Event event;
    };

private:
    int m_maxFps = 60;
    SDL_Window* m_mainWindow;
    SDL_GLContext m_mainContext;
    Handle<ISubscription> m_updateEvent;
    Handle<BasicLoopManager> m_basicLoopManager;
    EventBus m_eventBus;

    DeltaTime calcTimeDelta();
    void resizeWindow(int width, int height);
    void setAttribute(SDL_GLattr attribute, int value);
    bool setOpenGLAttributes();
    bool initSdl2(std::vector<std::string> args);
    SDL_Window* createWindow(std::string name, int width, int height);
    SDL_GLContext createGLContext(SDL_Window* window);
    void setSwapInterval(SDL_GLContext, int swapInterval);
    void initGlew(SDL_GLContext);
    void initWindow(std::string name, int width, int height);
    int getGLAttribute(SDL_GLattr attribute);
    void printSdlGlAttributes();
    void cleanup();
};

} // flappy
