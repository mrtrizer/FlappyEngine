#include "GLShaderResFactory.h"

#include <sstream>

#include <Entity.h>

#include <IFileLoadManager.h>
#include <IFileMonitorManager.h>
#include <GLShaderRes.h>
#include <ThreadManager.h>
#include <ResManager.h>

namespace flappy {

using namespace std;

GLShaderResFactory::GLShaderResFactory() {
    addDependency(IFileLoadManager::id());
    addDependency(IFileMonitorManager::id());
    addDependency(ResRepositoryManager::id());
    addDependency(ThreadManager::id());
}

std::shared_ptr<ResBase> GLShaderResFactory::load(const std::string& name)  {
    return create(name);
}

std::shared_ptr<ResBase> GLShaderResFactory::create(const std::string& name) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);

    auto vertexShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["vertex"]);
    auto vertexShader = manager<IFileLoadManager>()->loadTextFile(vertexShaderFileInfo.path);
    manager<IFileMonitorManager>()->registerFile(vertexShaderFileInfo.path);

    auto fragmentShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["fragment"]);
    auto fragmentShader = manager<IFileLoadManager>()->loadTextFile(fragmentShaderFileInfo.path);
    manager<IFileMonitorManager>()->registerFile(fragmentShaderFileInfo.path);

    auto rootEntity = manager<ThreadManager>()->entity();

    auto shaderRes = std::make_shared<GLShaderRes>(rootEntity, vertexShader, fragmentShader);
    shaderRes->initShader();
    return shaderRes;
}

bool GLShaderResFactory::changed(const string &name) {
    auto resMeta = manager<ResRepositoryManager>()->findResMeta(name);

    auto vertexShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["vertex"]);
    if (manager<IFileMonitorManager>()->changed(vertexShaderFileInfo.path))
        return true;

    auto fragmentShaderFileInfo = manager<ResRepositoryManager>()->findFileInfo(resMeta.data["fragment"]);
    if (manager<IFileMonitorManager>()->changed(fragmentShaderFileInfo.path))
        return true;
    return false;
}

} // flappy
