#pragma once

#include <map>
#include <unordered_map>
#include <list>

#include <Manager.h>

#include "ResKeeper.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

class Res;
class IResFactory;
class IFileMonitorManager;

class ResRepositoryManager final: public Manager<ResRepositoryManager>
{
public:
    struct ResInfo {
        std::string name;
        std::string path;
        std::string type;
    };

    ResRepositoryManager(std::string filePath);

    ResInfo findResInfo(std::string name) const;

    void init() override;
    void update(DeltaTime) override;

private:
    std::string m_filePath;
    std::map<std::string, ResInfo> m_resInfoMap;

    void loadResInfoList();
};

/// @}

} // flappy
