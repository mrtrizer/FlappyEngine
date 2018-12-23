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

ResRepositoryManager::ResRepositoryManager(Handle<Hierarchy> hierarchy)
    : Updatable<ResRepositoryManager>(hierarchy)
    , m_fileMonitorManager(hierarchy->manager<IFileMonitorManager>())
    , m_fileLoadManager(hierarchy->manager<IFileLoadManager>())
{
}
    
void ResRepositoryManager::setRepositoryPath(const std::string& path) {
    m_resRepositoryPath = path;
    auto fileListPath = m_fileListPath = StringUtils::joinPath({path, "file_list.json"});
    auto resMetaFilePath = m_resMetaFilePath = StringUtils::joinPath({path, "cache_meta.json"});
    m_fileMonitorManager->registerFile(fileListPath);
    m_fileMonitorManager->registerFile(resMetaFilePath);
    loadFileList(path, fileListPath);
    loadResMeta(path, resMetaFilePath);
}
    
void ResRepositoryManager::update(float dt) {
    if (m_fileMonitorManager->changed(m_fileListPath))
        loadFileList(m_resRepositoryPath, m_fileListPath);
    if (m_fileMonitorManager->changed(m_resMetaFilePath))
        loadResMeta(m_resRepositoryPath, m_resMetaFilePath);
}

void ResRepositoryManager::loadFileList(const std::string& repoPath, const std::string& filePath) {
    using namespace nlohmann;

    m_resInfoMap.clear();
    auto jsonStr = m_fileLoadManager->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        std::string name = jsonIterator.key();
        std::string path = jsonIterator.value().at("path");
        std::string type = jsonIterator.value().at("type");
        auto resPath = StringUtils::joinPath({repoPath, path});
        m_resInfoMap[name] = FileInfo {name, resPath, type};
    }
}

void ResRepositoryManager::loadResMeta(const std::string& repoPath, const std::string& filePath) {
    using namespace nlohmann;
    
    m_resMetaMap.clear();
    auto jsonStr = m_fileLoadManager->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        m_resMetaMap[jsonIterator.key()] = ResMeta { jsonToMap(jsonIterator.value()) };
    }
}

FileInfo ResRepositoryManager::findFileInfo(const std::string& name) const {
    USER_ASSERT_MSG(!m_resMetaFilePath.empty(), "Path for resources is not set up.");
    if (m_resInfoMap.find(name) != m_resInfoMap.end()) {
        return m_resInfoMap.at(name);
    } else {
        LOGE("Can't find information about resource in resource repository list. Name: %s", name.c_str());
        auto resPath = StringUtils::joinPath({m_resRepositoryPath, name});
        return FileInfo{ name, resPath, "*" };
    }
}

ResMeta ResRepositoryManager::findResMeta(const std::string& name) const {
    USER_ASSERT_MSG(!m_resMetaFilePath.empty(), "Path for resources is not set up.");
    if (m_resMetaMap.find(name) != m_resMetaMap.end()) {
        return m_resMetaMap.at(name);
    } else {
        LOGE("Can't find resource meta in resource repository list. Name: %s", name.c_str());
        return ResMeta();
    }
}

/// @}

} // flappy
