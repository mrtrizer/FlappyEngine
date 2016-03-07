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
#include <shapes/gobjrect.h>
#include <world.h>
#include <ctrl.h>

#include "glutmgr.h"

namespace GLUTMgr {

std::shared_ptr<GLWorldView> gWorldView;
std::shared_ptr<Ctrl> flappyCtrl;
std::shared_ptr<GLViewFactory> glViewFactory;

void render() {
    gWorldView->redrawWorld();
    glutSwapBuffers();
    glutPostRedisplay();
    flappyCtrl->step(); //only for test
}

void resizeWindow(int width, int height) {
    //I create new view for constructor/destructor testing
    gWorldView = std::make_shared<GLWorldView>(glViewFactory);
    flappyCtrl->setView(gWorldView);
    gWorldView->init();
    gWorldView->resize(width, height);
}

void mouseFunc(int button, int state,
               int x, int y) {
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        flappyCtrl->mouseClick(x,y);
}

void passiveMotionFunc(int x, int y) {
    flappyCtrl->mouseMove(x,y);
}

void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory) {
    GLUTMgr::glViewFactory = glViewFactory;
    flappyCtrl = std::make_shared<Ctrl>();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DOUBLE | GLUT_MULTISAMPLE);
    glutInitWindowSize(400, 600);
    glutCreateWindow("FlappyCxx");
    glewInit();

    flappyCtrl->init();

    glutMouseFunc(mouseFunc);
    glutPassiveMotionFunc(passiveMotionFunc);
    glutReshapeFunc(resizeWindow);
    glutDisplayFunc(render);
    glutMainLoop();
}

} //namespace GLUTMgr
