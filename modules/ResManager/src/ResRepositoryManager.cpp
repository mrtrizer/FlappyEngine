#include "ResRepositoryManager.h"

#include <json/json.hpp>

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

ResRepositoryManager::ResRepositoryManager(std::string resRespositoryPath)
    : m_resRepositoryPath(resRespositoryPath)
{
    addDependency(IFileLoadManager::id());
    addDependency(IFileMonitorManager::id());

    subscribe([this](InitEvent) {
        loadFileList();
        manager<IFileMonitorManager>()->registerFile(fileListFilePath());
        loadResMeta();
        manager<IFileMonitorManager>()->registerFile(resMetaFilePath());
    });

    subscribe([this](UpdateEvent) {
        if (manager<IFileMonitorManager>()->changed(fileListFilePath())) {
            loadFileList();
        }
        if (manager<IFileMonitorManager>()->changed(resMetaFilePath())) {
            loadResMeta();
        }
    });

}

std::string ResRepositoryManager::fileListFilePath() {
    return Tools::joinPath({m_resRepositoryPath, "file_list.json"});
}

std::string ResRepositoryManager::resMetaFilePath() {
    return Tools::joinPath({m_resRepositoryPath, "cache_meta.json"});
}

void ResRepositoryManager::loadFileList() {
    using namespace nlohmann;

    m_resInfoMap.clear();
    auto filePath = fileListFilePath();
    auto jsonStr = manager<IFileLoadManager>()->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        std::string name = jsonIterator.key();
        std::string path = jsonIterator.value().at("path");
        std::string type = jsonIterator.value().at("type");
        auto resPath = Tools::joinPath({m_resRepositoryPath, path});
        m_resInfoMap[name] = FileInfo {name, resPath, type};
    }
}

void ResRepositoryManager::loadResMeta() {
    using namespace nlohmann;

    m_resMetaMap.clear();
    auto filePath = resMetaFilePath();
    auto jsonStr = manager<IFileLoadManager>()->loadTextFile(filePath);
    auto json = json::parse(jsonStr);
    for (json::iterator jsonIterator = json.begin(); jsonIterator != json.end(); ++jsonIterator) {
        m_resMetaMap[jsonIterator.key()] = ResMeta { jsonToMap(jsonIterator.value()) };
    }
}

FileInfo ResRepositoryManager::findFileInfo(std::string name) const {
    if (m_resInfoMap.find(name) != m_resInfoMap.end()) {
        return m_resInfoMap.at(name);
    } else {
        LOGE("Can't find information about resource in resource repository list.");
        auto resPath = Tools::joinPath({m_resRepositoryPath, name});
        return FileInfo{ name, resPath, "*" };
    }
}

ResMeta ResRepositoryManager::findResMeta(std::string name) const {
    if (m_resMetaMap.find(name) != m_resMetaMap.end()) {
        return m_resMetaMap.at(name);
    } else {
        LOGE("Can't find resource meta in resource repository list.");
        return ResMeta();
    }
}

/// @}

} // flappy
