#include <cstdio>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GL/glut.h>

#include <gl/glworldview.h>
#include <core/gworldmodel.h>
#include <shapes/gobjcircle.h>
#include <core/gobjcamera.h>
#include <core/gworldctrl.h>
#include <shapes/gobjrect.h>
#include <gl/glviewfactory.h>

#include "glutmgr.h"

namespace GLUTMgr {

std::shared_ptr<GLWorldView> gWorldView;
std::shared_ptr<GWorldCtrl> gWorldCtrl;
std::shared_ptr<GLViewFactory> glViewFactory;

void render() {
    gWorldView->redrawWorld();
    glutSwapBuffers();
    glutPostRedisplay();
    gWorldCtrl->step(); //only for test
}

void resizeWindow(int width, int height) {
    //I create new view for constructor/destructor testing
    gWorldView = std::make_shared<GLWorldView>(glViewFactory);
    gWorldCtrl->setView(gWorldView);
    gWorldView->init();
    gWorldView->resize(width, height);
}

void mouseFunc(int button, int state,
               int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        gWorldCtrl->mouseClick(x,y);
}

void passiveMotionFunc(int x, int y) {
    gWorldCtrl->mouseMove(x,y);
}

void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory, std::shared_ptr<GWorldCtrl> gWorldCtrl) {
    GLUTMgr::glViewFactory = glViewFactory;
    GLUTMgr::gWorldCtrl = gWorldCtrl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(400, 600);
    glutCreateWindow("FlappyCxx");
    glewInit();

    gWorldCtrl->init();

    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);
    glutMainLoop();
}

} //namespace GLUTMgr
