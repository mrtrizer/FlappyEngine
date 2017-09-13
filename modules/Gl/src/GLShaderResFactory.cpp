#include "GLShaderResFactory.h"

#include <sstream>

#include <Entity.h>

#include <IFileLoadManager.h>
#include <GLShaderProgram.h>
#include <ResManager.h>

namespace flappy {

using namespace std;

GLShaderResFactory::GLShaderResFactory() {
    addDependency(IFileLoadManager::id());
    addDependency(ResRepositoryManager::id());
}

std::shared_ptr<Res> GLShaderResFactory::load(const std::string& name)  {
    return create(name);
}

std::shared_ptr<Res> GLShaderResFactory::create(const std::string& name) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);
    auto vertexShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["vertex"]);
    auto vertexShader = manager<IFileLoadManager>()->loadTextFile(vertexShaderFileInfo.path);
    auto fragmentShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["fragment"]);
    auto fragmentShader = manager<IFileLoadManager>()->loadTextFile(fragmentShaderFileInfo.path);
    auto shaderRes = std::make_shared<GLShaderProgram>(vertexShader, fragmentShader);
    shaderRes->initShader();
    return shaderRes;
}

} // flappy
