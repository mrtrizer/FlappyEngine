#include "GLTextureResFactory.h"

#include <sstream>

#include <Entity.h>
#include <ThreadManager.h>
#include <IRgbaBitmapRes.h>
#include <ResManager.h>
#include <GLTextureRes.h>

namespace flappy {

using namespace std;

GLTextureResFactory::GLTextureResFactory() {
    addDependency(ResManager<IRgbaBitmapRes>::id());
    addDependency(ThreadManager::id());
}

std::shared_ptr<ResBase> GLTextureResFactory::load(const std::string& name)  {
    return create(name);
}

std::shared_ptr<ResBase> GLTextureResFactory::create(const std::string& name) {
    auto rootEntity = manager<ThreadManager>()->entity();

    auto res = std::make_shared<GLTextureRes>(rootEntity, manager<ResManager<IRgbaBitmapRes>>()->getRes(name));
    return res;
}

} // flappy
