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
        m_resourceMap.emplace(path, make_shared<ResHandler<ResourceTD>>(path, std::move(newRes), shared_from_this()));
    }

    template <typename ResourceT>
    std::shared_ptr<ResHandler<ResourceT>> get(const std::string& path)
    {
        using namespace std;
        if (m_resourceMap.count(path) == 0)
            m_resourceMap.emplace(path, make_shared<ResHandler<ResourceT>>(path));
        return dynamic_pointer_cast<ResHandler<ResourceT>>(m_resourceMap[path]);
    }

    void update(TimeDelta) override;

    template <typename Type>
    std::unique_ptr<Type> load(const std::string& path) const
    {
        throw std::runtime_error(path + " Can't be loaded. Template was not specialized for this type of resources.");
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<IResHandler>> m_resourceMap;
    std::shared_ptr<IResourceLoader> m_resourceLoader;
};

template <typename ResourceT>
void ResHandler<ResourceT>::reloadFromSource(std::shared_ptr<ResManager> resManager)
{
    setNewResource(std::move(resManager->load<ResourceT>(m_name)), resManager);
}

template <>
void ResHandler<Atlas>::procNewResource(std::shared_ptr<ResManager> resManager);

template<>
std::unique_ptr<Texture> ResManager::load<Texture>(const std::string& path) const;

template<>
std::unique_ptr<Atlas> ResManager::load<Atlas>(const std::string& path) const;

template <>
std::shared_ptr<ResHandler<Quad>> ResManager::get<Quad>(const std::string& path);

} // flappy

#define RES_MGR MGR<ResManager>()
