#pragma once

#include <map>
#include <unordered_map>
#include <list>

#include <Hierarchy.hpp>
#include <Updatable.hpp>

#include "ResKeeper.h"
#include "ResInfo.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

class IResFactory;
class IFileMonitorManager;
class IFileLoadManager;

class [[manager]] ResRepositoryManager : public Updatable<ResRepositoryManager>
{
public:
    ResRepositoryManager(Handle<Hierarchy> hierarchy);

    void setRepositoryPath(const std::string& path);
    
    void update(float dt);
    
    FileInfo findFileInfo(const std::string& name) const;
    ResMeta findResMeta(const std::string& name) const;

private:
    std::string m_resRepositoryPath;
    Handle<IFileMonitorManager> m_fileMonitorManager;
    Handle<IFileLoadManager> m_fileLoadManager;
    std::map<std::string, FileInfo> m_resInfoMap;
    std::map<std::string, ResMeta> m_resMetaMap;
    std::string m_fileListPath;
    std::string m_resMetaFilePath;

    void loadFileList(const std::string& repoPath, const std::string& filePath);
    void loadResMeta(const std::string& repoPath, const std::string& filePath);
};

/// @}

} // flappy
