#include "GlutManager.h"
#include "GlutInit.h"

namespace flappy {

GlutManager::GlutManager()
{

}

GlutManager::init()
{
    GLUTInit::initGLUT(entity());
}

} // flappy
