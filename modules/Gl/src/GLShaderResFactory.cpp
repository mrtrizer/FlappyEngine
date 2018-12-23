#include "GLShaderResFactory.h"

#include <sstream>

#include <IFileLoadManager.h>
#include <IFileMonitorManager.h>
#include <GLShaderRes.h>
#include <ResManager.h>

namespace flappy {

using namespace std;

GLShaderResFactory::GLShaderResFactory(Handle<Hierarchy> hierarchy)
    : m_fileLoadManager(hierarchy->manager<IFileLoadManager>())
    , m_fileMonitorManager(hierarchy->manager<IFileMonitorManager>())
    , m_resRepositoryManager(hierarchy->manager<ResRepositoryManager>())
    , m_rootEntity(hierarchy->rootEntity())
{}

std::shared_ptr<ResBase> GLShaderResFactory::load(const std::string& name, ExecType)  {
    return create(name);
}

std::shared_ptr<ResBase> GLShaderResFactory::create(const std::string& name) {
    auto resMeta = m_resRepositoryManager->findResMeta(name);

    auto vertexShaderFileInfo = m_resRepositoryManager->findFileInfo(resMeta.data["vertex"]);
    auto vertexShader = m_fileLoadManager->loadTextFile(vertexShaderFileInfo.path);
    m_fileMonitorManager->registerFile(vertexShaderFileInfo.path);

    auto fragmentShaderFileInfo = m_resRepositoryManager->findFileInfo(resMeta.data["fragment"]);
    auto fragmentShader = m_fileLoadManager->loadTextFile(fragmentShaderFileInfo.path);
    m_fileMonitorManager->registerFile(fragmentShaderFileInfo.path);

    auto shaderRes = std::make_shared<GLShaderRes>(m_rootEntity, vertexShader, fragmentShader);
    shaderRes->initShader();
    return shaderRes;
}

bool GLShaderResFactory::changed(const string &name) {
    auto resMeta = m_resRepositoryManager->findResMeta(name);

    auto vertexShaderFileInfo = m_resRepositoryManager->findFileInfo(resMeta.data["vertex"]);
    if (m_fileMonitorManager->changed(vertexShaderFileInfo.path))
        return true;

    auto fragmentShaderFileInfo = m_resRepositoryManager->findFileInfo(resMeta.data["fragment"]);
    if (m_fileMonitorManager->changed(fragmentShaderFileInfo.path))
        return true;
    return false;
}

} // flappy
