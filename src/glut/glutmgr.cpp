#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

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

#include "glutmgr.h"

namespace GLUTMgr {

std::shared_ptr<GLViewMgr> gWorldView;
std::shared_ptr<GLViewFactory> glViewFactory;

void render() {
    glutSwapBuffers();
    glutPostRedisplay();
    FlappyApp::inst().update();
}

void resizeWindow(int width, int height) {
    //I create new view for constructor/destructor testing
    gWorldView = std::make_shared<GLViewMgr>(glViewFactory);
    gWorldView->init();
    gWorldView->resize(width, height);
}

void mouseFunc(int button, int state, int x, int y) {
    FlappyApp::inst().inputMgr()->mouseMove(glm::vec3(x,y,0));
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        FlappyApp::inst().inputMgr()->setMouseDown();
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        FlappyApp::inst().inputMgr()->setMouseUp();
}

void passiveMotionFunc(int x, int y) {
    FlappyApp::inst().inputMgr()->mouseMove(glm::vec3(x,y,0));
}

void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory) {
    GLUTMgr::glViewFactory = glViewFactory;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    //TODO: Ways to control size of window
    glutInitWindowSize(600, 600);
    glutCreateWindow("FlappyCxx");
#ifdef __GLEW_H__
    glewInit();
#endif

    // TODO: Order is important
    FlappyApp::inst().configure();
    FlappyApp::inst().setWorldView(std::make_shared<GLViewMgr>(glViewFactory));

    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);
    glutMainLoop();
}

} //namespace GLUTMgr
