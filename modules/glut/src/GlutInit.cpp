#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#warning FIXME: Strange path
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
#include <CameraComponent.h>
#include <InputManager.h>
#include <AppManager.h>
#include <GLViewCircle.h>
#include <GLViewRect.h>
#include <GLViewSprite.h>

#include "GlutInit.h"

namespace flappy {

class CircleShape;
class RectShape;
class SpriteComponent;

namespace GLUTInit {

using namespace std;

shared_ptr<ManagerList> g_managerList;
char** g_argv = nullptr;
int g_argc = 0;

void render() {
    glutSwapBuffers();
    glutPostRedisplay();
    g_managerList->update();
}

void resizeWindow(int width, int height) {
    g_managerList->MGR<ViewManager>()->resize(width, height);
}

void mouseFunc(int button, int state, int x, int y) {
    g_managerList->MGR<InputManager>()->setMousePos({x,y});
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        g_managerList->MGR<InputManager>()->setMouseDown();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        g_managerList->MGR<InputManager>()->setMouseUp();
}

void passiveMotionFunc(int x, int y) {
    g_managerList->MGR<InputManager>()->setMousePos({x,y});
}

void initGLUT(shared_ptr<ManagerList> managerList) {
    GLUTInit::g_managerList = managerList;

    auto args = managerList->MGR<AppManager>()->args();
    g_argv = new char*[args.size()];
    for (unsigned i = 0; i < args.size(); i++) {
        g_argv[i] = new char[args[i].size()];
        std::strcpy(g_argv[i],args[i].data());
    }
    g_argc = args.size();
    glutInit(&g_argc, g_argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    //TODO: Ways to control size of window
    glutInitWindowSize(600, 600);
    glutCreateWindow("FlappyCxx");
#ifdef __GLEW_H__
    glewInit();
#endif

    auto manager = managerList->override<ViewManager, GLViewManager>();
    manager->bind<CircleShape,GLViewCircle>();
    manager->bind<RectShape,GLViewRect>();
    manager->bind<SpriteComponent,GLViewSprite>();

    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);

}

int mainLoop() {
    glutMainLoop();
    return 0;
}

} // GLUTManager

} // flappy
