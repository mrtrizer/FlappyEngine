#include "glviewmanager.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>
#include <core/view.h>
#include <core/presenter.h>
#include <core/screenmanager.h>

#include "glshaderprogram.h"
#include "glshaderprogram.h"
#include "gltools.h"
#include "glattribarray.h"
#include "glviewfactory.h"

namespace flappy {

using namespace glm;
using namespace std;

void GLViewManager::init() {
    resize(100,100);
    LOGI("OpenGL Version: %s\n", glGetString(GL_VERSION));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    CHECK_GL_ERROR;
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL_ERROR;
    glEnable (GL_BLEND);
    CHECK_GL_ERROR;
    updateSize();
}

GLViewManager::~GLViewManager() {

}

void GLViewManager::redraw(list<Visual> &presenterList, mat4 &pMatrix) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR;

    //sort presenters by z (I use z value defined once on object creation)
        presenterList.sort([](const Visual & first, const Visual & second) {
        return first.z < second.z;
    });

    //and draw presenters one by one appying move matrices
    for (auto presenter: presenterList) {
        auto mvMatrix = presenter.pos;
        presenter.presenter->getGView(*m_factory)->redraw(pMatrix, mvMatrix);
    }
}

void GLViewManager::updateViewPort() {
    glViewport(0, 0, MGR<ScreenManager>()->width(), MGR<ScreenManager>()->height());
    CHECK_GL_ERROR;
}

} // flappy
