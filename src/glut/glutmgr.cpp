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

#include <gl/glworldview.h>
#include <shapes/gobjcircle.h>
#include <core/gobjcamera.h>
#include <shapes/gobjrect.h>
#include <gl/glviewfactory.h>
#include <core/initsystem.h>

#include "glutmgr.h"

namespace GLUTMgr {

std::shared_ptr<GLWorldView> gWorldView;
std::shared_ptr<GLViewFactory> glViewFactory;

class Application : public entityx::EntityX {
public:
    explicit Application() {
        systems.add<GLWorldView>(GLUTMgr::glViewFactory);
        systems.add<InitSystem>();
        systems.configure();
    }

    void update(entityx::TimeDelta dt) {
        systems.update_all(dt);
    }
};

std::shared_ptr<Application> app;

void render() {
    glutSwapBuffers();
    glutPostRedisplay();
    app->update(0.1);
}

void resizeWindow(int width, int height) {
    //I create new view for constructor/destructor testing
    gWorldView = std::make_shared<GLWorldView>(glViewFactory);
    gWorldView->init();
    gWorldView->resize(width, height);
}


void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory) {
    GLUTMgr::glViewFactory = glViewFactory;

    app = std::make_shared<Application>();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(400, 600);
    glutCreateWindow("FlappyCxx");
#ifdef __GLEW_H__
    glewInit();
#endif

    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);
    glutMainLoop();
}

} //namespace GLUTMgr
