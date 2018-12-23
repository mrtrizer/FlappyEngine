#include "GLTextureResFactory.h"

#include <sstream>

#include <IRgbaBitmapRes.h>
#include <ResManager.h>
#include <GLTextureRes.h>

namespace flappy {

using namespace std;

GLTextureResFactory::GLTextureResFactory(Handle<Hierarchy> hierarchy)
    : m_bitmapResManager(hierarchy->manager<ResManager<IRgbaBitmapRes>>())
    , m_rootEntity(hierarchy->rootEntity())
{}

std::shared_ptr<ResBase> GLTextureResFactory::load(const std::string& name, ExecType execType)  {
    auto bitmapRes = m_bitmapResManager->getRes(name, execType);
    auto res = std::make_shared<GLTextureRes>(m_rootEntity, bitmapRes);
    return res;
}

std::shared_ptr<ResBase> GLTextureResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
