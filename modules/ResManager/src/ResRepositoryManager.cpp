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
    : m_resRepositoryPath(resRespositoryPath)
{
    addDependency(IFileLoadManager::id());
    addDependency(IFileMonitorManager::id());
}

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
        std::string name = jsonIterator->at("name");
        std::string path = jsonIterator->at("path");
        std::string type = jsonIterator->at("type");
        auto resPath = Tools::joinPath({m_resRepositoryPath, path});
        m_resInfoMap[name] = ResInfo {name, resPath, type};
    }
}

void ResRepositoryManager::init() {
    loadResInfoList();
    manager<IFileMonitorManager>()->registerFile(m_resRepositoryPath);
}

ResInfo ResRepositoryManager::findResInfo(std::string name) const {
    if (m_resInfoMap.find(name) != m_resInfoMap.end()) {
        return m_resInfoMap.at(name);
    } else {
        LOGE("Can't find information about resource in resource repository list.");
        auto resPath = Tools::joinPath({m_resRepositoryPath, name});
        return ResInfo{name, resPath, "*"};
    }
}

void ResRepositoryManager::update(DeltaTime)
{
    auto resListPath = resInfoListFilePath();
    if (manager<IFileMonitorManager>()->changed(resListPath)) {
        loadResInfoList();
    }
}

/// @}

} // flappy
