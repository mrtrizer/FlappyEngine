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
                    resPairIter->second.updateRes(this->template manager<ResFactory<ResT>>(), name, this->template manager<IFileMonitorManager>());
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

    /// @brief Set resource with name. Resource will be destroyed in next update
    /// if it hasn't external links.
    void setRes(const std::string& name, std::shared_ptr<ResT> res);

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
        try { // catch all exception from default res create function
            auto defaultRes = this->template manager<ResFactory<ResT>>()->create(name);
            auto resKeeper = ResKeeper(defaultRes);
            auto iter = m_resMap.emplace(name, std::move(resKeeper)).first;
            return iter->second;
        } catch (std::exception& e) {
            throw std::runtime_error(std::string("Default resource create error.\nDescription:\n") + e.what());
        }
        catch (...) {
            throw std::runtime_error("Default resource create error.");
        }
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
        resKeeper.updateRes(this->template manager<ResFactory<ResT>>(), name, this->template manager<IFileMonitorManager>());
    }
    return std::static_pointer_cast<ResT>(resKeeper.actualRes());
}

/// @brief Set resource with name. Resource will be destroyed in next update
/// if it hasn't external links.
template<typename ResT>
void ResManager<ResT>::setRes(const std::string& name, std::shared_ptr<ResT> res)
{
    auto foundIter = m_resMap.find(name);
    if (foundIter == m_resMap.end())
        m_resMap.emplace(name, ResKeeper(res, false));
    else
        foundIter->second.actualRes()->pushRes(res);
}

/// @}

} // flappy
