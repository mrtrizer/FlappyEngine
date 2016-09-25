#pragma once

#include <unordered_map>
#include <list>

#include "manager.h"
#include "iresourceloader.h"
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

class ResManager: public Manager, public std::enable_shared_from_this<ResManager>
{
    template <typename ResT>
    friend class ResHandler;

public:

    template <typename ResT>
    void bind(std::shared_ptr<IResFactory> factory) {
        const unsigned resId = ClassId<IRes, ResT>::id();
        if (m_factories.size() <= resId)
            m_factories.resize(resId + 1);
        m_factories[resId] = factory;
    }

    template <typename ResourceT>
    void set(const std::string& path, ResourceT& resource)
    {
        using namespace std;

        const unsigned resId = ClassId<IRes, ResourceT>::id();

        if (m_resourceMap.size() <= resId)
            m_resourceMap.resize(resId + 1);

        using ResourceTD = typename decay<ResourceT>::type;
        auto newRes = make_shared<ResourceTD>(forward<ResourceT>(resource));
        auto resHandler = make_shared<ResHandler<ResourceTD>>(path);
        resHandler->setNewResource(newRes);

        auto factory = m_factories[resId];
        if (!factory)
            return ERROR_MSG(VOID_VALUE,"Resource type is not registered!");

        factory->initHandler(resHandler, newRes, shared_from_this());

        m_resourceMap[resId].emplace(path, std::move(resHandler));
    }

    template <typename ResourceT>
    std::shared_ptr<ResHandler<ResourceT>> get(const std::string& path)
    {
        using namespace std;

        const unsigned resId = ClassId<IRes, ResourceT>::id();

        //TODO: Factory length check

        if (m_resourceMap.size() <= resId)
            m_resourceMap.resize(resId + 1);

        auto factory = m_factories[resId];
        if (!factory)
            return ERROR_MSG(nullptr,"Resource type is not registered!");

        if (m_resourceMap[resId].count(path) == 0) {
            auto resHandler = make_shared<ResHandler<ResourceT>>(path);
            factory->initRes(resHandler, shared_from_this());
            m_resourceMap[resId].emplace(path, resHandler);
        }
        return dynamic_pointer_cast<ResHandler<ResourceT>>(m_resourceMap[resId][path]);
    }

    void update(TimeDelta) override;

private:
    std::vector<std::unordered_map<std::string, std::shared_ptr<IResHandler>>> m_resourceMap;
    std::vector<std::shared_ptr<IResFactory>> m_factories;

};

} // flappy

#define RES_MGR MGR<ResManager>()
