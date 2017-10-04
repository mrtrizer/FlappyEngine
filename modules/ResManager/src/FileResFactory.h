#pragma once

#include <string>

#include "ResFactory.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"
#include "ResRepositoryManager.h"
#include "TextRes.h"

namespace flappy {

class IFileMonitorManager;
class Entity;

/// @addtogroup ResManager
/// @{

/// Base class for file based resource factory.
/// @details File based resources have no dependencies.
/// Usially, they also supports function of auto-update.
/// To support auto-update of resource, pass implementation
/// of file monitor to constructor.
class FileResFactory : public ResFactory<TextRes>
{
public:
    FileResFactory();

    std::shared_ptr<ResBase> load(const std::string& name) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    bool changed(const std::string& name) final;
};

FileResFactory::FileResFactory()
{
    this->addDependency(ResRepositoryManager::id());
    this->addDependency(IFileMonitorManager::id());
    this->addDependency(IFileLoadManager::id());
}

std::shared_ptr<ResBase> FileResFactory::load(const std::string& name) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    auto fileLoadManager = this->template manager<IFileLoadManager>();
    return std::make_shared<TextRes>(fileLoadManager->loadTextFile(fileInfo.path));
}

std::shared_ptr<ResBase> FileResFactory::create(const std::string& name) {
    return std::make_shared<TextRes>("");
}

bool FileResFactory::changed(const std::string& name) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    return this->template manager<IFileMonitorManager>()->changed(fileInfo.path);
}


/// @}

} // flappy
