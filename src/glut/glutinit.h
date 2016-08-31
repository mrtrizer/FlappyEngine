#pragma once

#include <memory>

namespace flappy {

class GLViewFactory;
class ManagerList;

namespace GLUTInit {
    void initGLUT(std::shared_ptr<ManagerList> managerList);
    int mainLoop();
}

} // flappy
