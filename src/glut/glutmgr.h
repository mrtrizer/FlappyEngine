#ifndef GLUTMGR_H
#define GLUTMGR_H

#include <memory>

#include <core/flappyapp.h>

class GLViewFactory;

namespace GLUTMgr {
    void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory, std::shared_ptr<FlappyApp> flappyApp);
}

#endif // GLUTMGR_H
