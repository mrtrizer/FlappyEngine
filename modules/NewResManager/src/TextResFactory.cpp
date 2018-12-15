#include "TextResFactory.h"
#include <Hierarchy.hpp>

namespace flappy {

TextResFactory::TextResFactory(Handle<Hierarchy> hierarchy)
    : m_resRepoManager(hierarchy->manager<ResRepositoryManager>())
    , m_fileLoadManager(hierarchy->manager<IFileLoadManager>())
    , m_fileMonitorManager(hierarchy->manager<IFileMonitorManager>())
{

}

std::shared_ptr<ResBase> TextResFactory::load(const std::string& name, ExecType) {
    auto resMeta = m_resRepoManager->findResMeta(name);
    auto fileInfo = m_resRepoManager->findFileInfo(resMeta.data["input"]);
    return std::make_shared<TextRes>(m_fileLoadManager->loadTextFile(fileInfo.path));
}

std::shared_ptr<ResBase> TextResFactory::create(const std::string& name) {
    return std::make_shared<TextRes>("");
}

bool TextResFactory::changed(const std::string& name) {
    auto resMeta = m_resRepoManager->findResMeta(name);
    auto fileInfo = m_resRepoManager->findFileInfo(resMeta.data["input"]);
    return m_fileMonitorManager->changed(fileInfo.path);
}

} // flappy
