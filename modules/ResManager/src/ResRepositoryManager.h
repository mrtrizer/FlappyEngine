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

class Res;
class IResFactory;
class IFileMonitorManager;

class ResRepositoryManager: public Manager<ResRepositoryManager>
{
public:
    ResRepositoryManager(std::string resRespositoryPath);

    ResInfo findResInfo(std::string name) const;

    void init() override;
    void update(DeltaTime) override;

private:
    std::string m_resRepositoryPath;
    std::map<std::string, ResInfo> m_resInfoMap;

    void loadResInfoList();
    std::string resInfoListFilePath();
};

/// @}

} // flappy
