#ifndef GLUTMGR_H
#define GLUTMGR_H

#include <memory>

class GWorldCtrl;
class GLViewFactory;

namespace GLUTMgr {
    void initGLUT(int argc, char** argv, std::shared_ptr<GLViewFactory> glViewFactory, std::shared_ptr<GWorldCtrl> gWorldCtrl);
}

#endif // GLUTMGR_H
