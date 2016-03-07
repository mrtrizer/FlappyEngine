#include "glworldview.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glshaderprogram.h"
#include "core/gworldmodel.h"
#include "core/gview.h"
#include "core/gpresenter.h"
#include "glshaderprogram.h"
#include "gltools.h"
#include "glattribarray.h"
#include "glviewfactory.h"

using namespace std;

void GLWorldView::init() {
    LOGI("OpenGL Version: %s\n", glGetString(GL_VERSION));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    CHECK_GL_ERROR;
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL_ERROR;
    glEnable (GL_BLEND);
    CHECK_GL_ERROR;
    updateSize();
}

GLWorldView::~GLWorldView() {

}

void GLWorldView::redraw(GPresenterList &presenterList, GTools::PMatrix &pMatrix) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR;

    //sort presenters by z (I use z value defined once on object creation)
    presenterList.sort([](const GObj::GObjP & first, const GObj::GObjP & second) {
        return first->getStaticZ() < second->getStaticZ();
    });

    //and draw presenters one by one appying move matrices
    for (auto presenter: presenterList) {
        auto mvMatrix = presenter->getAPos().getMvMatrix();
        presenter->getGView(*factory)->redraw(pMatrix.data(), mvMatrix.data());
    }

}

void GLWorldView::updateViewPort(int width, int height) {
    glViewport(0, 0, width, height);
    CHECK_GL_ERROR;
}
