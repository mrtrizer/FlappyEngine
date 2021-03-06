#include "GLRenderManager.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/gtc/type_ptr.hpp>

#include <Render.h>
#include <ScreenManager.h>
#include <IGLManager.h>
#include <DebugServices.h>

#include "GLShaderRes.h"
#include "GLTools.h"
#include "GLAttribArray.h"

namespace flappy {

using namespace glm;
using namespace std;

GLRenderManager::GLRenderManager(Handle<Hierarchy> hierarchy)
    : RenderManager(hierarchy)
    , m_glManager(hierarchy->manager<IGLManager>())
    , m_screenManager(hierarchy->manager<ScreenManager>())
{
    LOGI("OpenGL Version: %s\n", glGetString(GL_VERSION));
    glClearColor(0.0, 0.0, 0.0, 0.0);
    CHECK_GL_ERROR;
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    CHECK_GL_ERROR;
    glEnable (GL_BLEND);
    CHECK_GL_ERROR;
    updateViewPort();
}

void GLRenderManager::redraw(list<Visual> &presenterList, mat4 &pMatrix) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    CHECK_GL_ERROR;

    //sort presenters by z (I use z value defined once on object creation)
    presenterList.sort([](const Visual & first, const Visual & second) {
        return first.z < second.z;
    });

    //and draw presenters one by one appying move matrices
    for (auto presenter: presenterList) {
        auto mvMatrix = presenter.pos;
        try {
            presenter.view->redraw(pMatrix, mvMatrix);
        } catch (std::exception& e) {
            LOGE("Rendering exception: %s", e.what());
        }
    }
}

void GLRenderManager::updateViewPort() {
    glViewport(0, 0, m_screenManager->screenSize().x, m_screenManager->screenSize().y);
    CHECK_GL_ERROR;
}

} // flappy
