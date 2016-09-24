#pragma once

#include <unordered_map>
#include <list>

#include "manager.h"
#include "iresourceloader.h"

#include "reshandler.h"

namespace flappy {

class Texture;
class Atlas;
class Quad;

class ResManager: public Manager, public std::enable_shared_from_this<ResManager>
{
    template <typename ResT>
    friend class ResHandler;

public:
    ResManager(std::shared_ptr<IResourceLoader> resourceLoader):
        m_resourceLoader(resourceLoader)
    {}

    template <typename ResourceT>
    void set(const std::string& path, ResourceT&& resource)
    {
        using namespace std;
        using ResourceTD = typename decay<ResourceT>::type;
        auto newRes = unique_ptr<ResourceTD>(new ResourceTD(forward<ResourceT>(resource)));
        auto resHandler = make_shared<ResHandler<ResourceTD>>(path);
        resHandler->setNewResource(std::move(newRes), shared_from_this());
        m_resourceMap.emplace(path, std::move(resHandler));
    }

    template <typename ResourceT>
    std::shared_ptr<ResHandler<ResourceT>> get(const std::string& path)
    {
        using namespace std;
        if (m_resourceMap.count(path) == 0) {
            auto resHandler = make_shared<ResHandler<ResourceT>>(path);
            initRes<ResourceT>(resHandler);
            m_resourceMap.emplace(path, resHandler);
        }
        return dynamic_pointer_cast<ResHandler<ResourceT>>(m_resourceMap[path]);
    }

    void update(TimeDelta) override;

private:
    std::unordered_map<std::string, std::shared_ptr<IResHandler>> m_resourceMap;
    std::shared_ptr<IResourceLoader> m_resourceLoader;

    template <typename Type>
    std::unique_ptr<Type> load(const std::string& path) const
    {
        throw std::runtime_error(path + " Can't be loaded. Template was not specialized for this type of resources.");
        return nullptr;
    }

    template <typename ResourceT>
    void initRes(std::shared_ptr<ResHandler<ResourceT>>) {

    }
};

template<>
std::unique_ptr<Texture> ResManager::load<Texture>(const std::string& path) const;

template<>
std::unique_ptr<Atlas> ResManager::load<Atlas>(const std::string& path) const;

template <>
void ResManager::initRes<Quad>(std::shared_ptr<ResHandler<Quad>>);

} // flappy

#define RES_MGR MGR<ResManager>()
