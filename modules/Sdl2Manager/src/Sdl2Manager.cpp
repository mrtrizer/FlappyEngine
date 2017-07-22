#include "Sdl2Manager.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <GLIncludes.h>
#include <GLTools.h>
#include <GLViewManager.h>
#include <AppManager.h>
#include <Entity.h>

#include "Sdl2Utils.h"

namespace flappy {

Sdl2Manager::Sdl2Manager()
    : AGLManager({AppManager::id()})
{}

void Sdl2Manager::resizeWindow(int width, int height) {
    ViewManager::OnWindowResize onWindowResizeEvent;
    onWindowResizeEvent.width = width;
    onWindowResizeEvent.height = height;
    events()->post(onWindowResizeEvent);
}

DeltaTime Sdl2Manager::calcTimeDelta() {
    using namespace std::chrono;
    auto newTime = steady_clock::now();
    auto diff = newTime - m_lastTime;
    DeltaTime timeDelta = diff.count(); // delta in seconds
    m_lastTime = newTime;
    return timeDelta;
}

void Sdl2Manager::init()
{
    if (initSdl2(manager<AppManager>()->args())) {
        initWindow("FlappyEngine", 600, 600);
    }
}

bool Sdl2Manager::initSdl2(std::vector<std::string> args) {
    auto argv = new char*[args.size()];
    for (unsigned i = 0; i < args.size(); i++) {
        argv[i] = new char[args[i].size()];
        std::strcpy(argv[i],args[i].data());
    }
    int argc = int(args.size());

    return true;
}

bool Sdl2Manager::setOpenGLAttributes()
{
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}

void Sdl2Manager::initWindow(std::string name, int width, int height)
{
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        LOGE("Failed to init SDL");

    CHECK_SDL_ERROR;

    // Create our window centered at 512x512 resolution
    m_mainWindow = SDL_CreateWindow(name.c_str(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width,
                                  height,
                                  SDL_WINDOW_OPENGL);

    CHECK_SDL_ERROR;

    // Check that everything worked out okay
    if (!m_mainWindow)
        LOGE("Unable to create window");

    // Create our opengl context and attach it to our window
    m_mainContext = SDL_GL_CreateContext(m_mainWindow);

    CHECK_SDL_ERROR;

    setOpenGLAttributes();

    CHECK_SDL_ERROR;

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    CHECK_SDL_ERROR;

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
    #endif
}

void Sdl2Manager::cleanup()
{
    // Delete our OpengL context
    SDL_GL_DeleteContext(m_mainContext);

    CHECK_SDL_ERROR;

    // Destroy our window
    SDL_DestroyWindow(m_mainWindow);

    CHECK_SDL_ERROR;

    // Shutdown SDL 2
    SDL_Quit();

    CHECK_SDL_ERROR;
}

void Sdl2Manager::printSdlGlAttributes()
{
    int majorVersion = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &majorVersion);

    int minorVersion = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &minorVersion);

    LOGI("SDL GL Context version: %d.%d", majorVersion, minorVersion);
}

int Sdl2Manager::startMainLoop()
{
    printSdlGlAttributes();

    bool loop = true;

    while( loop)
    {
        // If frames were 30, wait 33 ms before running the loop again
        usleep( 1000000/60 );

        auto updateEvent = ComponentBase::OnUpdate();
        updateEvent.dt = calcTimeDelta();
        entity()->events()->post(updateEvent);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) {
                loop = false;
                break;
            } else {
                events()->post(Sdl2Event(event));
            }
        }

           // Swap the buffers.
        SDL_GL_SwapWindow(m_mainWindow);

        CHECK_SDL_ERROR;
    }

    cleanup();
    return 0;
}

} // flappy
