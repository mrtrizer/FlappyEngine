#include "ResManager.h"
#include "ResFactory.h"
#include "Res.h"

namespace flappy {

using namespace std;

ResKeeper& ResManager::ResType::getResKeeper(const string& name, shared_ptr<ResManager> resManager)
{
    auto resIter = resMap.find(name);
    if (resIter == resMap.end()) {
        if (resFactory == nullptr)
            throw runtime_error("ResFactory is not binded");
        try { // catch all exception from default res create function
            auto defaultRes = resFactory->create(name, resManager);
            auto resKeeper = ResKeeper(defaultRes);
            auto iter = resMap.emplace(name, std::move(resKeeper)).first;
            return iter->second;
        } catch (exception& e) {
            throw runtime_error(string("Default resource create error.\nDescription:\n") + e.what());
        }
        catch (...) {
            throw runtime_error("Default resource create error.");
        }
    } else {
        return resIter->second;
    }
}

std::shared_ptr<Res> ResManager::ResType::getRes(const string& name, shared_ptr<ResManager> resManager) {
    return getResKeeper(name, resManager).actualRes();
}

void ResManager::update(TimeDelta)
{
    for (auto& resType: m_resTypeVector) {
        if (resType.resFactory == nullptr)
            continue;
        for (auto resPairIter = resType.resMap.begin(); resPairIter != resType.resMap.end();) {
            resPairIter->second.cleanUpRes();
            if (resPairIter->second.needRemove()) {
                resPairIter = resType.resMap.erase(resPairIter);
            } else {
                resPairIter->second.updateRes(resType.resFactory, resPairIter->first, shared_from_this());
                resPairIter++;
            }
        }
    }
}

} // flappy
