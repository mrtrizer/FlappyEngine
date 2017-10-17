#include "Sdl2Manager.h"

#include <cstdio>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <GLIncludes.h>
#include <GLTools.h>
#include <GLRenderManager.h>
#include <Entity.h>
#include <ThreadManager.h>
#include <Application.h>

#include "Sdl2Utils.h"

namespace flappy {

Sdl2Manager::Sdl2Manager()
{
    addDependency(ThreadManager::id());

    subscribe([this](InitEvent) {
        initWindow("FlappyEngine", 600, 600);

        m_updateEvent = subscribe([this](UpdateEvent) {
            update();
        });
    });

    subscribe([this](DeinitEvent) {
        cleanup();

        unsubscribe(m_updateEvent);
    });

}

bool Sdl2Manager::isReady() const {
    return entity()->parent() == nullptr;
}

void Sdl2Manager::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event) == 1)
    {
        if (event.type == SDL_QUIT) {
            manager<ThreadManager>()->quit();
            break;
        } else {
            entity()->events()->post(Sdl2Event(event));
        }
    }

    // Swap the buffers.
    SDL_GL_SwapWindow(m_mainWindow);
    TRACE_SDL_ERRORS;
}

void Sdl2Manager::resizeWindow(int width, int height) {
    RenderManager::OnWindowResize onWindowResizeEvent;
    onWindowResizeEvent.width = width;
    onWindowResizeEvent.height = height;
    events()->post(onWindowResizeEvent);
}

void Sdl2Manager::setAttribute(SDL_GLattr attribute, int value)
{
    int statusCode = SDL_GL_SetAttribute(attribute, value);
    TRACE_SDL_ERRORS;
    if (statusCode < 0)
        throw std::runtime_error("Failed to set attribute");
}

bool Sdl2Manager::setOpenGLAttributes()
{
    // Set our OpenGL version.
    // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
//    setAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // I'm using verion 2.1 for now. Version 3.2 is not supported yet
    setAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    setAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    // Turn on double buffering with a 24bit Z buffer.
    // You may need to change this to 16 or 32 for your system
    setAttribute(SDL_GL_DOUBLEBUFFER, 1);

    return true;
}

SDL_Window* Sdl2Manager::createWindow(std::string name, int width, int height)
{
    // We should set attributes BEFORE initialization of WINDOW
    setOpenGLAttributes();

    // Create our window centered
    auto mainWindow = SDL_CreateWindow(name.c_str(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width,
                                  height,
                                  SDL_WINDOW_OPENGL);

    TRACE_SDL_ERRORS;

    // Check that everything worked out okay
    if (mainWindow == nullptr)
        throw std::runtime_error("Unable to create window");
    return mainWindow;
}

SDL_GLContext Sdl2Manager::createGLContext(SDL_Window* window) {
    auto mainContext = SDL_GL_CreateContext(window);

    TRACE_SDL_ERRORS;

    if (mainContext == NULL)
        throw std::runtime_error("Unable to create context");

    return mainContext;
}

void Sdl2Manager::setSwapInterval(SDL_GLContext, int swapInterval)
{
    int statusCode = SDL_GL_SetSwapInterval(swapInterval);

    TRACE_SDL_ERRORS;

    if (statusCode == -1)
        throw std::runtime_error("Swap interval is not supported");
}

void Sdl2Manager::initGlew(SDL_GLContext) {
    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum statusCode = glewInit();
    if (statusCode != GLEW_OK) {
        LOGE("Error: %s", glewGetErrorString(statusCode));
        throw std::runtime_error("Glew initialization failed");
    }
    #endif
}

void Sdl2Manager::initWindow(std::string name, int width, int height)
{
    // Initialize SDL's Video subsystem
    {
        int statusCode = SDL_Init(SDL_INIT_VIDEO);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
        TRACE_SDL_ERRORS;

        if (statusCode < 0)
            throw std::runtime_error("Failed to init SDL");
    }

    auto mainWindow = createWindow(name, width, height);

    // Create our opengl context and attach it to our window
    auto glContext = createGLContext(mainWindow);

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    setSwapInterval(glContext, 1);

    initGlew(glContext);

    printSdlGlAttributes();

    m_mainWindow = mainWindow;
    m_mainContext = glContext;
}

void Sdl2Manager::cleanup()
{
    // Delete our OpengL context
    SDL_GL_DeleteContext(m_mainContext);
    TRACE_SDL_ERRORS;

    // Destroy our window
    SDL_DestroyWindow(m_mainWindow);
    TRACE_SDL_ERRORS;

    // Shutdown SDL 2
    SDL_Quit();
    TRACE_SDL_ERRORS;
}

int Sdl2Manager::getGLAttribute(SDL_GLattr attribute) {
    int result = 0;
    int statusCode = SDL_GL_GetAttribute(attribute, &result);
    if (statusCode < 0)
        throw std::runtime_error("Can't get GL context attribute");
    return result;
}

void Sdl2Manager::printSdlGlAttributes()
{
    int majorVersion = getGLAttribute(SDL_GL_CONTEXT_MAJOR_VERSION);
    int minorVersion = getGLAttribute(SDL_GL_CONTEXT_MINOR_VERSION);

    LOGI("SDL GL Context version: %d.%d", majorVersion, minorVersion);
}

} // flappy
