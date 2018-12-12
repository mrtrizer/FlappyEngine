#pragma once

#include <map>
#include <unordered_map>
#include <list>

#include <Manager.h>

#include "ResKeeper.h"
#include "ResInfo.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

class IResFactory;
class IFileMonitorManager;

class ResRepositoryManager: public Manager<ResRepositoryManager>
{
public:
    ResRepositoryManager(std::string resRespositoryPath);

    FileInfo findFileInfo(std::string name) const;
    ResMeta findResMeta(std::string name) const;

private:
    std::string m_resRepositoryPath;
    std::map<std::string, FileInfo> m_resInfoMap;
    std::map<std::string, ResMeta> m_resMetaMap;

    void loadFileList();
    void loadResMeta();
    std::string fileListFilePath();
    std::string resMetaFilePath();
};

/// @}

} // flappy
