#pragma once

#include <string>

#include "IResFactory.h"
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
template <typename ResT>
class FileResFactory : public ResFactory<ResT>
{
public:
    FileResFactory(std::string path = "", std::string ext = "");

    std::shared_ptr<Res> load(const std::string& name) override;
    std::shared_ptr<Res> create(const std::string& name) override;
    bool changed(const std::string& name) override;

private:
    std::string m_path;
    std::string m_ext;
};

template <typename ResT>
FileResFactory<ResT>::FileResFactory(std::string path, std::string ext):
    m_path(path),
    m_ext(ext)
{
    this->addDependency(ResRepositoryManager::id());
    this->addDependency(IFileMonitorManager::id());
    this->addDependency(IFileLoadManager::id());
}

template <typename ResT>
std::shared_ptr<Res> FileResFactory<ResT>::load(const std::string& name) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    auto fileLoadManager = this->template manager<IFileLoadManager>();
    return std::make_shared<TextRes>(fileLoadManager->loadTextFile(fileInfo.path));
}

template <typename ResT>
std::shared_ptr<Res> FileResFactory<ResT>::create(const std::string& name) {
    return std::make_shared<TextRes>("");
}

template <typename ResT>
bool FileResFactory<ResT>::changed(const std::string& name) {
    auto resMeta = this->template manager<ResRepositoryManager>()->findResMeta(name);
    auto fileInfo = this->template manager<ResRepositoryManager>()->findFileInfo(resMeta.data["input"]);
    return this->template manager<IFileMonitorManager>()->changed(fileInfo.path);
}


/// @}

} // flappy
