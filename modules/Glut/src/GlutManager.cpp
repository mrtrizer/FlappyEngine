#include "GlutManager.h"

#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

// FIXME: Strange path
#include <GLTools.h> //glew has to be included before glut

#define GLUT_NOT_SUPPORTED "GLUT is not supported on target platform"

#if defined(_WIN32)
  #error GLUT_NOT_SUPPORTED
#elif defined(__APPLE__)
  #include "TargetConditionals.h"
  #if TARGET_OS_IPHONE == 1
    #error GLUT_NOT_SUPPORTED
  #elif TARGET_OS_MAC == 1
    #include <GLUT/glut.h>
  #else
    #error GLUT_NOT_SUPPORTED
  #endif
#elif defined(ANDROID_JNI)
  #error GLUT_NOT_SUPPORTED
#elif defined(__unix__)
  #include <GL/glut.h>
#endif

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
        if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            InputManager::OnMouseDown onMouseDownEvent;
            onMouseDownEvent.x = x;
            onMouseDownEvent.y = y;
            onMouseDownEvent.button = InputManager::MouseButton::LEFT;
            g_entity->events()->post(onMouseDownEvent);
        }
        if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
            InputManager::OnMouseUp onMouseUpEvent;
            onMouseUpEvent.x = x;
            onMouseUpEvent.y = y;
            onMouseUpEvent.button = InputManager::MouseButton::LEFT;
            g_entity->events()->post(onMouseUpEvent);
        }
    }

    void resizeWindow(int width, int height) {
        ViewManager::OnWindowResize onWindowResizeEvent;
        onWindowResizeEvent.width = width;
        onWindowResizeEvent.height = height;
        g_entity->events()->post(onWindowResizeEvent);
        glutPostRedisplay();
    }

    void render() {
        glutSwapBuffers();
    }

    void timer(int e) {
        auto updateEvent = Component::OnUpdate();
        updateEvent.dt = calcTimeDelta();
        g_entity->events()->post(updateEvent);

        // Redraw screen with new object data.
        glutPostRedisplay();

        // Timer is a one shoot must be reset after being called.
        // By using a timed event, your application should run about the same speed on any machine.
        glutTimerFunc(g_updateDelayMs, timer, 0);
    }
}

GlutManager::GlutManager() :AGLManager({ClassId<Component,AppManager>().id()}) {

}

void GlutManager::init()
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

bool GlutManager::initGlut(std::vector<std::string> args) {
    auto argv = new char*[args.size()];
    for (unsigned i = 0; i < args.size(); i++) {
        argv[i] = new char[args[i].size()];
        std::strcpy(argv[i],args[i].data());
    }
    int argc = int(args.size());
    glutInit(&argc, argv);

    return true;
}

int GlutManager::initWindow(std::string name, int width, int height)
{
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);

    glutInitWindowSize(width, height);
    int windowId = glutCreateWindow(name.c_str());
#ifdef __GLEW_H__
    glewInit();
#endif

    glutMouseFunc(mouseFunc);
    glutMotionFunc(passiveMotionFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);
    glutTimerFunc(g_updateDelayMs, timer, 0);

    return windowId;
}

int GlutManager::startMainLoop()
{
    glutMainLoop();
    return 0;
}

} // flappy
