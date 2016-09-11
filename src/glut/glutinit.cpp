#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

#include <gl/gltools.h> //glew has to be included before glut

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

#include <gl/glviewmgr.h>
#include <core/camera.h>
#include <gl/glviewfactory.h>
#include <core/inputmgr.h>
#include <core/appmgr.h>

#include "glutinit.h"

namespace flappy {

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
    //I create new view for constructor/destructor testing
    auto viewMgr = make_shared<GLViewMgr>(make_shared<GLViewFactory>());
    g_managerList->add(viewMgr);
    viewMgr->init();
    viewMgr->resize(width, height);
}

void mouseFunc(int button, int state, int x, int y) {
    g_managerList->MGR<InputMgr>()->mouseMove(glm::vec3(x,y,0));
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        g_managerList->MGR<InputMgr>()->setMouseDown();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        g_managerList->MGR<InputMgr>()->setMouseUp();
}

void passiveMotionFunc(int x, int y) {
    g_managerList->MGR<InputMgr>()->mouseMove(glm::vec3(x,y,0));
}

void initGLUT(shared_ptr<ManagerList> managerList) {
    GLUTInit::g_managerList = managerList;

    auto args = managerList->MGR<AppMgr>()->args();
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

    managerList->create<GLViewMgr>(make_shared<GLViewFactory>());

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

} // GLUTMgr

} // flappy
