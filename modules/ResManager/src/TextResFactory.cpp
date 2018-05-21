#include "TextResFactory.h"

#include <Entity.h>


namespace flappy {

TextResFactory::TextResFactory()
{
    this->addDependency(ResRepositoryManager::id());
    this->addDependency(IFileMonitorManager::id());
    this->addDependency(IFileLoadManager::id());
}

std::shared_ptr<ResBase> TextResFactory::load(const std::string& name, ExecType) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    auto fileLoadManager = this->template manager<IFileLoadManager>();
    return std::make_shared<TextRes>(fileLoadManager->loadTextFile(fileInfo.path));
}

std::shared_ptr<ResBase> TextResFactory::create(const std::string& name) {
    return std::make_shared<TextRes>("");
}

bool TextResFactory::changed(const std::string& name) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    return this->template manager<IFileMonitorManager>()->changed(fileInfo.path);
}

} // flappy