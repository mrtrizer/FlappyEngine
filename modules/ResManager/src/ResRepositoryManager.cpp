#include "ResRepositoryManager.h"

#include <json/json.hpp>
#include <DebugServices.h>
#include <StringUtils.hpp>

#include "Res.h"
#include "IFileLoadManager.h"
#include "IFileMonitorManager.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

namespace {

std::unordered_map<std::string, std::string> jsonToMap(const nlohmann::json &jsonMap) {
    using namespace nlohmann;

    std::unordered_map<std::string, std::string> resultMap;

    for (json::const_iterator jsonIterator = jsonMap.begin(); jsonIterator != jsonMap.end(); ++jsonIterator) {
        resultMap[jsonIterator.key()] = jsonIterator.value();
    }

    return resultMap;
}

}

ResRepositoryManager::ResRepositoryManager(Handle<Hierarchy> hierarchy, std::string resRespositoryPath)
    : Updatable<ResRepositoryManager>(hierarchy)
    , m_resRepositoryPath(resRespositoryPath)
    , m_fileMonitorManager(hierarchy->manager<IFileMonitorManager>())
    , m_fileLoadManager(hierarchy->manager<IFileLoadManager>())
{
    loadFileList();
    m_fileMonitorManager->registerFile(fileListFilePath());
    loadResMeta();
    m_fileMonitorManager->registerFile(resMetaFilePath());
}
    
void ResRepositoryManager::update(float dt) {
    if (m_fileMonitorManager->changed(fileListFilePath())) {
        loadFileList();
    }
    if (m_fileMonitorManager->changed(resMetaFilePath())) {
        loadResMeta();
    }
}

std::string ResRepositoryManager::fileListFilePath() {
    return StringUtils::joinPath({m_resRepositoryPath, "file_list.json"});
}

std::string ResRepositoryManager::resMetaFilePath() {
    return StringUtils::joinPath({m_resRepositoryPath, "cache_meta.json"});
}

void ResRepositoryManager::loadFileList() {
    using namespace nlohmann;

    m_resInfoMap.clear();
    auto filePath = fileListFilePath();
    auto jsonStr = m_fileLoadManager->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        std::string name = jsonIterator.key();
        std::string path = jsonIterator.value().at("path");
        std::string type = jsonIterator.value().at("type");
        auto resPath = StringUtils::joinPath({m_resRepositoryPath, path});
        m_resInfoMap[name] = FileInfo {name, resPath, type};
    }
}

void ResRepositoryManager::loadResMeta() {
    using namespace nlohmann;

    m_resMetaMap.clear();
    auto filePath = resMetaFilePath();
    auto jsonStr = m_fileLoadManager->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        m_resMetaMap[jsonIterator.key()] = ResMeta { jsonToMap(jsonIterator.value()) };
    }
}

FileInfo ResRepositoryManager::findFileInfo(std::string name) const {
    if (m_resInfoMap.find(name) != m_resInfoMap.end()) {
        return m_resInfoMap.at(name);
    } else {
        LOGE("Can't find information about resource in resource repository list. Name: %s", name.c_str());
        auto resPath = StringUtils::joinPath({m_resRepositoryPath, name});
        return FileInfo{ name, resPath, "*" };
    }
}

ResMeta ResRepositoryManager::findResMeta(std::string name) const {
    if (m_resMetaMap.find(name) != m_resMetaMap.end()) {
        return m_resMetaMap.at(name);
    } else {
        LOGE("Can't find resource meta in resource repository list. Name: %s", name.c_str());
        return ResMeta();
    }
}

/// @}

} // flappy
