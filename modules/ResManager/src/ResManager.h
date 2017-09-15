#pragma once

#include <map>
#include <unordered_map>
#include <list>

#include <Res.h>
#include <Manager.h>
#include <ResRepositoryManager.h>

#include "IFileMonitorManager.h"
#include "ResKeeper.h"
#include "IResFactory.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

template <typename ResT>
class ResManager final: public Manager<ResManager<ResT>>
{
public:
    ResManager()
    {
        this->addDependency(ResRepositoryManager::id());
        this->addDependency(ResFactory<ResT>::id());
        this->addDependency(IFileMonitorManager::id());

        this->subscribe([this](ComponentBase::UpdateEvent) {
            for (auto resPairIter = m_resMap.begin(); resPairIter != m_resMap.end();) {
                resPairIter->second.cleanUpRes();
                if (resPairIter->second.needRemove()) {
                    resPairIter = m_resMap.erase(resPairIter);
                } else {
                    auto name = resPairIter->first;
                    resPairIter->second.updateRes();
                    resPairIter++;
                }
            }
        });
    }

    /// @brief If resource is not loaded yet, method returns default
    /// resource, created by binded ResourceFactory.
    /// If resource is not loaded and ResFactory is not binded,
    /// runtime_error exception will be generated.
    /// @return Pointer to inialized resource.
    /// @see ResourceFactory, ResManager::bindResFactory(), ResManager::setRes()
    std::shared_ptr<ResT> getRes(const std::string& name);

    /// @brief Synchronous version of getRes
    std::shared_ptr<ResT> getResSync(const std::string& name);

    /// @brief Bind resource factory, used to load and reload resources.
    /// Resource can't be initlalized and loaded without binded factory.
    /// But it still can be set with setRes() and got with getRes() later.
    void bindResFactory(std::shared_ptr<IResFactory> factory);

private:
    std::map<std::string, ResKeeper> m_resMap;

    ResKeeper& getResKeeper(const std::string& name);
};


template<typename ResT>
ResKeeper& ResManager<ResT>::getResKeeper(const std::string& name)
{
    auto resIter = m_resMap.find(name);
    if (resIter == m_resMap.end()) {
        auto resKeeper = ResKeeper(this->template manager<ResFactory<ResT>>(), name);
        auto iter = m_resMap.emplace(name, std::move(resKeeper)).first;
        return iter->second;
    } else {
        return resIter->second;
    }
}

template<typename ResT>
std::shared_ptr<ResT> ResManager<ResT>::getRes(const std::string& name) {
    if (!this->template isInitialized())
        throw std::runtime_error(std::string("ResManager for ") + typeName<ResT>() + " is not initialized.");
    return std::static_pointer_cast<ResT>(getResKeeper(name).actualRes());
}

/// @brief Synchronous version of getRes
template<typename ResT>
std::shared_ptr<ResT> ResManager<ResT>::getResSync(const std::string& name)
{
    auto& resKeeper = getResKeeper(name);
    if (this->isInitialized()) {
        resKeeper.updateRes();
    }
    return std::static_pointer_cast<ResT>(resKeeper.actualRes());
}

/// @}

} // flappy
