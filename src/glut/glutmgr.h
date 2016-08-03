#pragma once

#include <memory>

#include <core/flappyapp.h>

namespace flappy {

class GLViewFactory;

namespace GLUTMgr {
    void initGLUT(int argc, char** argv, shared_ptr<GLViewFactory> glViewFactory);
}

} // flappy
