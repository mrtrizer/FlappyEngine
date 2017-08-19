#include "ResRepositoryManager.h"

#include <json/json.hpp>

#include "ResFactory.h"
#include "Res.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

using namespace std;

ResRepositoryManager::ResRepositoryManager(std::string filePath)
    : Manager({IFileLoadManager::id(), IFileMonitorManager::id()})
    , m_filePath(filePath)
{}

void ResRepositoryManager::loadResInfoList() {
    using namespace nlohmann;

    m_resInfoMap.clear();
    auto jsonStr = manager<IFileLoadManager>()->loadTextFile(m_filePath);
    auto json = json::parse(jsonStr);
    auto jsonResInfoList = json["list"];
    for (json::iterator jsonIterator = jsonResInfoList.begin(); jsonIterator != jsonResInfoList.end(); ++jsonIterator) {
        std::string path = jsonIterator->at("path");
        std::string type = jsonIterator->at("type");
        m_resInfoMap[path] = ResInfo {path, path, type};
    }
}

void ResRepositoryManager::init() {
    loadResInfoList();
    manager<IFileMonitorManager>()->registerFile(m_filePath);
}

ResRepositoryManager::ResInfo ResRepositoryManager::findResInfo(std::string name) const {
    return m_resInfoMap.at(name);
}

void ResRepositoryManager::update(DeltaTime)
{
    if (manager<IFileMonitorManager>()->changed(m_filePath)) {
        loadResInfoList();
    }
}

/// @}

} // flappy
