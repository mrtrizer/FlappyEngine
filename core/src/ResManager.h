#pragma once

#include <unordered_map>
#include <list>

#include <Manager.h>

#include "ResManager/ResKeeper.h"

namespace flappy {

/// @addtogroup ResManager
/// @{

class Res;
class ResFactory;

class ResManager final: public Manager, public std::enable_shared_from_this<ResManager>
{
public:
    ResManager():m_resTypeVector(ClassCounter<Res>::count()) {}
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
    template <typename ResT>
    std::shared_ptr<ResT> getRes(const std::string& name)
    {
        return std::static_pointer_cast<ResT>(getResType<ResT>().getRes(name, shared_from_this()));
    }

    /// @brief Set resource with name. Resource will be destroyed in next update
    /// if it hasn't external links.
    template <typename ResT>
    void setRes(const std::string& name, std::shared_ptr<ResT> res)
    {
        getResType<ResT>().resMap.emplace(name, ResKeeper(res, false));
    }

    /// @brief Bind resource factory, used to load and reload resources.
    /// Resource can't be initlalized and loaded without binded factory.
    /// But it still can be set with setRes() and got with getRes() later.
    template <typename ResT>
    void bindResFactory(std::shared_ptr<ResFactory> factory)
    {
        getResType<ResT>().resFactory = factory;
    }

    void update(TimeDelta) override;

private:
    struct ResType
    {
        std::shared_ptr<ResFactory> resFactory;
        std::map<std::string, ResKeeper> resMap;

        /// @brief Creates default resource with ResFactory::create()
        /// if can't find existing resource.
        /// Generates runtime_error exception if ResFactory is not set.
        /// @return Initialized resource by name
        std::shared_ptr<Res> getRes(const std::string& name, std::shared_ptr<ResManager>);
    };

    std::vector<ResType> m_resTypeVector;

    /// @return ResType reference. Resize m_resTypeVector if need.
    template <typename ResT>
    ResType& getResType()
    {
        return m_resTypeVector[ClassId<Res, ResT>::id()];
    }
};

/// @}

} // flappy
