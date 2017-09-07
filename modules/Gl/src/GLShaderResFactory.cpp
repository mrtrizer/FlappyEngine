#include "GLShaderResFactory.h"

#include <sstream>

#include <Entity.h>

#include <GLShaderProgram.h>
#include <ResManager.h>

namespace flappy {

using namespace std;

GLShaderResFactory::GLShaderResFactory() {
    addDependency(ResManager<TextRes>::id());
}

std::shared_ptr<Res> GLShaderResFactory::load(const std::string& name)  {
    return create(name);
}

std::shared_ptr<Res> GLShaderResFactory::create(const std::string& name) {
    auto textResFactory = manager<ResManager<TextRes>>();
    auto shaderRes = std::make_shared<GLShaderProgram>(
                textResFactory->getResSync(name + "_vertex"),
                textResFactory->getResSync(name + "_fragment"));
    shaderRes->initShader();
    return shaderRes;
}

} // flappy
