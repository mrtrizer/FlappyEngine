#include "GLShaderResFactory.h"

#include <sstream>

#include <Entity.h>

#include <GLShaderProgram.h>
#include <ResManager.h>

namespace flappy {

using namespace std;

std::shared_ptr<Res> GLShaderResFactory::load(const ResInfo& resInfo, SafePtr<Entity> entity)  {
    return create(resInfo.name, entity);
}

std::shared_ptr<Res> GLShaderResFactory::create(const std::string& name, SafePtr<Entity> entity) {
    auto textResFactory = entity->manager<ResManager<TextRes>>();
    return std::make_shared<GLShaderProgram>(
                textResFactory->getResSync(name + "_vertex"),
                textResFactory->getResSync(name + "_fragment"));
}

} // flappy
