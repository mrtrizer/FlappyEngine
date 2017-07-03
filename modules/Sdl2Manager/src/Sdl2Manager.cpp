#include "Sdl2Manager.h"

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

// FIXME: Strange path
#include <GLTools.h> //glew has to be included before glut

#include <SDL2/SDL.h>

#include <GLViewManager.h>
#include <InputManager.h>
#include <AppManager.h>
#include <GLViewCircle.h>
#include <GLViewRect.h>
#include <GLViewSprite.h>

namespace flappy {

namespace {

    int g_updateDelayMs = 16; // ~60 fps
    SafePtr<Entity> g_entity;
    std::chrono::steady_clock::time_point g_lastTime;

    // Our SDL_Window ( just like with SDL2 wihout OpenGL)
    SDL_Window *mainWindow;

    // Our opengl context handle
    SDL_GLContext mainContext;

    DeltaTime calcTimeDelta() {
        using namespace std::chrono;
        auto newTime = steady_clock::now();
        auto diff = newTime - g_lastTime;
        DeltaTime timeDelta = diff.count(); // delta in seconds
        g_lastTime = newTime;
        return timeDelta;
    }

    void passiveMotionFunc(int x, int y) {
        InputManager::OnMouseMove onMouseMoveEvent;
        onMouseMoveEvent.x = x;
        onMouseMoveEvent.y = y;
        g_entity->events()->post(onMouseMoveEvent);
    }

    void mouseFunc(int button, int state, int x, int y) {
        passiveMotionFunc(x, y);
        //if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            InputManager::OnMouseDown onMouseDownEvent;
            onMouseDownEvent.x = x;
            onMouseDownEvent.y = y;
            onMouseDownEvent.button = InputManager::MouseButton::LEFT;
            g_entity->events()->post(onMouseDownEvent);
        //}
    }

    void resizeWindow(int width, int height) {
        ViewManager::OnWindowResize onWindowResizeEvent;
        onWindowResizeEvent.width = width;
        onWindowResizeEvent.height = height;
        g_entity->events()->post(onWindowResizeEvent);
    }

    void timer(int e) {
        auto updateEvent = Component::OnUpdate();
        updateEvent.dt = calcTimeDelta();
        g_entity->events()->post(updateEvent);

    }
}

Sdl2Manager::Sdl2Manager() :AGLManager({AppManager::id()}) {}

void Sdl2Manager::init()
{
    // Global pointer to Entity is used in glut callbacks
    if (g_entity != nullptr)
        throw std::runtime_error("Can't initialize glut twice.");
    g_entity = entity();

    if (initGlut(manager<AppManager>()->args())) {
        int glutWindowId = initWindow("FlappyEngine", 600, 600);
        if (glutWindowId > 0)
            m_glutWindowId = glutWindowId;
    }
}

bool Sdl2Manager::initGlut(std::vector<std::string> args) {
    auto argv = new char*[args.size()];
    for (unsigned i = 0; i < args.size(); i++) {
        argv[i] = new char[args[i].size()];
        std::strcpy(argv[i],args[i].data());
    }
    int argc = int(args.size());

    return true;
}

bool SetOpenGLAttributes()
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

int Sdl2Manager::initWindow(std::string name, int width, int height)
{
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        LOGE("Failed to init SDL");

    // Create our window centered at 512x512 resolution
    mainWindow = SDL_CreateWindow(name.c_str(),
                                  SDL_WINDOWPOS_CENTERED,
                                  SDL_WINDOWPOS_CENTERED,
                                  width,
                                  height,
                                  SDL_WINDOW_OPENGL);

    // Check that everything worked out okay
    if (!mainWindow)
        LOGE("Unable to create window");

    // Create our opengl context and attach it to our window
    mainContext = SDL_GL_CreateContext(mainWindow);

    SetOpenGLAttributes();

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Init GLEW
    // Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
    #ifndef __APPLE__
    glewExperimental = GL_TRUE;
    glewInit();
    #endif

    return 0;
}

void RunGame()
{
    bool loop = true;

    while( loop)
    {
        // If frames were 30, wait 33 ms before running the loop again
        usleep( 1000000/60 );

        auto updateEvent = Component::OnUpdate();
        updateEvent.dt = calcTimeDelta();
        g_entity->events()->post(updateEvent);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) {
                loop = false;
                break;
            }
        }

           // Swap the buffers.
        SDL_GL_SwapWindow(mainWindow);
    }
}

void Cleanup()
{
    // Delete our OpengL context
    SDL_GL_DeleteContext(mainContext);

    // Destroy our window
    SDL_DestroyWindow(mainWindow);

    // Shutdown SDL 2
    SDL_Quit();
}


int Sdl2Manager::startMainLoop()
{
    RunGame();
    Cleanup();
    return 0;
}

void CheckSDLError(int line = -1)
{
    std::string error = SDL_GetError();

    if (error != "")
    {
        std::cout << "SLD Error : " << error << std::endl;

        if (line != -1)
            std::cout << "\nLine : " << line << std::endl;

        SDL_ClearError();
    }
}

void PrintSDL_GL_Attributes()
{
    int value = 0;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
    std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

} // flappy
