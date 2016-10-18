#pragma once

#include <unordered_map>
#include <list>

#include "manager.h"
#include "res.h"
#include "reshandler.h"

namespace flappy {

class Texture;
class Atlas;
class Quad;

class ResManager;

class IResFactory {
public:
    virtual std::shared_ptr<IRes> load(const std::string&) { return nullptr; }
    virtual void initRes(std::shared_ptr<IResHandler>, std::shared_ptr<ResManager>) {}
    virtual void initHandler(std::shared_ptr<IResHandler>, std::shared_ptr<IRes>, std::shared_ptr<ResManager>) {}
};

class ResManager final: public Manager<>, public std::enable_shared_from_this<ResManager>
{
    template <typename ResT>
    friend class ResHandler;

public:
    ResManager() = default;
    ResManager(const ResManager&) = delete;
    ResManager(ResManager&&) = default;
    ~ResManager() = default;
    ResManager& operator=(const ResManager&) = delete;
    ResManager& operator=(ResManager&&) = default;

    template <typename ResT>
    void bind(std::shared_ptr<IResFactory> factory) {
        const unsigned resId = ClassId<IRes, ResT>::id();
        Tools::resizeAndGet(m_factories, resId) = factory;
    }

    template <typename ResT>
    auto set(const std::string& path, ResT&& resource)
    {
        using namespace std;

        const unsigned resId = ClassId<IRes, decay_t<ResT>>::id();

        auto& resList = Tools::resizeAndGet(m_resourceMaps, resId);


        auto newRes = make_shared<decay_t<ResT>>(forward<ResT>(resource));
        shared_ptr<ResHandler<decay_t<ResT>>> resHandler;
        auto handlerIter = resList.find(path);
        if (handlerIter == resList.end()) {
            resHandler = make_shared<ResHandler<decay_t<ResT>>>(path);
            resList.emplace(path, resHandler);
        } else {
            resHandler = static_pointer_cast<ResHandler<decay_t<ResT>>>(handlerIter->second);
        }
        resHandler->setNewResource(newRes);

        if (auto factory = Tools::resizeAndGet(m_factories, resId))
            factory->initHandler(resHandler, newRes, shared_from_this());

        return resHandler;
    }

    template <typename ResT>
    std::shared_ptr<ResHandler<ResT>> get(const std::string& path)
    {
        using namespace std;

        const unsigned resId = ClassId<IRes, ResT>::id();

        if (m_resourceMaps.size() <= resId)
            m_resourceMaps.resize(resId + 1);

        if (m_resourceMaps[resId].count(path) == 0) {
            auto resHandler = make_shared<ResHandler<ResT>>(path);
            if (auto factory = Tools::resizeAndGet(m_factories, resId))
                factory->initRes(resHandler, shared_from_this());
            m_resourceMaps[resId][path] = resHandler;
        }
        return static_pointer_cast<ResHandler<ResT>>(m_resourceMaps[resId][path]);
    }

    void update(TimeDelta) override;

private:
    std::vector<std::unordered_map<std::string, std::shared_ptr<IResHandler>>> m_resourceMaps;
    std::vector<std::shared_ptr<IResFactory>> m_factories;
};

} // flappy

#define RES_MGR MGR<ResManager>()
