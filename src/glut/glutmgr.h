#ifndef GLUTMGR_H
#define GLUTMGR_H

#include <memory>

class GLViewFactory;

namespace GLUTMgr {
    void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory);
}

#endif // GLUTMGR_H
