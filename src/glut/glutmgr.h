#pragma once

#include <memory>

#include <core/flappyapp.h>

namespace flappy {

class GLViewFactory;

namespace GLUTMgr {
    void initGLUT(int argc, char **argv, std::shared_ptr<FlappyApp> flappyApp);
    int mainLoop();
}

} // flappy
