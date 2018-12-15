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
    ResRepositoryManager(Handle<Hierarchy> hierarchy, std::string resRespositoryPath);

    void update(float dt);
    
    FileInfo findFileInfo(std::string name) const;
    ResMeta findResMeta(std::string name) const;

private:
    std::string m_resRepositoryPath;
    Handle<IFileMonitorManager> m_fileMonitorManager;
    Handle<IFileLoadManager> m_fileLoadManager;
    std::map<std::string, FileInfo> m_resInfoMap;
    std::map<std::string, ResMeta> m_resMetaMap;

    void loadFileList();
    void loadResMeta();
    std::string fileListFilePath();
    std::string resMetaFilePath();
};

/// @}

} // flappy
