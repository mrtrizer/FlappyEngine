#include "ResRepositoryManager.h"

#include <json/json.hpp>

#include "IResFactory.h"
#include "Res.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

using namespace std;

ResRepositoryManager::ResRepositoryManager(std::string resRespositoryPath)
    : Manager({IFileLoadManager::id(), IFileMonitorManager::id()})
    , m_resRepositoryPath(resRespositoryPath)
{}

std::string ResRepositoryManager::resInfoListFilePath() {
    return Tools::joinPath({m_resRepositoryPath, "res_list.json"});
}

void ResRepositoryManager::loadResInfoList() {
    using namespace nlohmann;

    m_resInfoMap.clear();
    auto resInfoListPath = resInfoListFilePath();
    auto jsonStr = manager<IFileLoadManager>()->loadTextFile(resInfoListPath);
    auto json = json::parse(jsonStr);
    auto jsonResInfoList = json["list"];
    for (json::iterator jsonIterator = jsonResInfoList.begin(); jsonIterator != jsonResInfoList.end(); ++jsonIterator) {
        // TODO: Rename path field to name field in resource info json file
        std::string path = jsonIterator->at("path");
        std::string type = jsonIterator->at("type");
        auto resPath = Tools::joinPath({m_resRepositoryPath, path});
        m_resInfoMap[path] = ResInfo {path, resPath, type};
    }
}

void ResRepositoryManager::init() {
    loadResInfoList();
    manager<IFileMonitorManager>()->registerFile(m_resRepositoryPath);
}

ResInfo ResRepositoryManager::findResInfo(std::string name) const {
    return m_resInfoMap.at(name);
}

void ResRepositoryManager::update(DeltaTime)
{
    if (manager<IFileMonitorManager>()->changed(m_resRepositoryPath)) {
        loadResInfoList();
    }
}

/// @}

} // flappy
