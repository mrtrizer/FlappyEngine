#pragma once

#include <map>
#include <unordered_map>
#include <list>

#include <Manager.h>

#include "ResKeeper.h"
#include "IResFactory.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

class Res;

template <typename ResT>
class ResManager final: public Manager<ResManager<ResT>>
{
public:
    ResManager() {}
    virtual ~ResManager() = default;
    ResManager(const ResManager&) = delete;
    ResManager& operator=(const ResManager&) = delete;
    ResManager(ResManager&&) = default;
    ResManager& operator=(ResManager&&) = default;

    /// @brief If resource is not loaded yet, method returns default
    /// resource, created by binded ResourceFactory.
    /// If resource is not loaded and ResFactory is not binded,
    /// runtime_error exception will be generated.
    /// @return Pointer to inialized resource.
    /// @see ResourceFactory, ResManager::bindResFactory(), ResManager::setRes()
    std::shared_ptr<ResT> getRes(const std::string& name);

    /// @brief Synchronous version of getRes
    std::shared_ptr<ResT> getResSync(const std::string& name)
    {
        auto& resKeeper = getResKeeper(name, this->entity());
        if (m_resFactory)
            resKeeper.updateRes(m_resFactory, name, this->entity());
        return std::static_pointer_cast<ResT>(resKeeper.actualRes());
    }

    /// @brief Set resource with name. Resource will be destroyed in next update
    /// if it hasn't external links.
    void setRes(const std::string& name, std::shared_ptr<ResT> res)
    {
        auto foundIter = m_resMap.find(name);
        if (foundIter == m_resMap.end())
            m_resMap.emplace(name, ResKeeper(res, false));
        else
            foundIter->second.actualRes()->pushRes(res);
    }

    /// @brief Bind resource factory, used to load and reload resources.
    /// Resource can't be initlalized and loaded without binded factory.
    /// But it still can be set with setRes() and got with getRes() later.
    void bindResFactory(std::shared_ptr<IResFactory> factory)
    {
        m_resFactory = factory;
    }

    void update(DeltaTime) override;

private:

    std::shared_ptr<IResFactory> m_resFactory;
    std::map<std::string, ResKeeper> m_resMap;

    /// @brief Creates default resource with ResFactory::create()
    /// if can't find existing resource.
    /// Generates runtime_error exception if ResFactory is not set.
    /// @return Initialized resource by name
    std::shared_ptr<Res> getRes(const std::string& name, SafePtr<Entity> entity);
    ResKeeper& getResKeeper(const std::string& name, SafePtr<Entity> entity);
};


template<typename ResT>
ResKeeper& ResManager<ResT>::getResKeeper(const std::string& name, SafePtr<Entity> entity)
{
    auto resIter = m_resMap.find(name);
    if (resIter == m_resMap.end()) {
        if (m_resFactory == nullptr)
            throw std::runtime_error("ResFactory is not binded");
        try { // catch all exception from default res create function
            auto defaultRes = m_resFactory->create(name, entity);
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
    return std::static_pointer_cast<ResT>(getResKeeper(name, this->entity()).actualRes());
}

template<typename ResT>
void ResManager<ResT>::update(DeltaTime)
{
    if (m_resFactory == nullptr)
        return;
    for (auto resPairIter = m_resMap.begin(); resPairIter != m_resMap.end();) {
        resPairIter->second.cleanUpRes();
        if (resPairIter->second.needRemove()) {
            resPairIter = m_resMap.erase(resPairIter);
        } else {
            resPairIter->second.updateRes(m_resFactory, resPairIter->first, this->entity());
            resPairIter++;
        }
    }
}

/// @}

} // flappy
