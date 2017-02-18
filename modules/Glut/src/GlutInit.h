#pragma once

#include <memory>

namespace flappy {

class GLViewFactory;
class Entity;

namespace GLUTInit {
    void initGLUT(std::shared_ptr<Entity> managerList);
    int mainLoop();
}

} // flappy
