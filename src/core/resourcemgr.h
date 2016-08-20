#pragma once

#include <unordered_map>
#include <list>

#include <core/manager.h>
#include <core/iresourceloader.h>
#include <core/atlas.h>
#include <core/quad.h>

namespace flappy {

class Texture;
class Atlas;
class ResourceMgr;

class IResourceHandler {
public:
    virtual ~IResourceHandler() noexcept {}
    virtual void update() = 0;
    virtual bool ready() const = 0;
    virtual bool updated() const = 0;
    virtual bool markedReload() const = 0;
    virtual void reloadFromSource(std::shared_ptr<ResourceMgr> resourceMgr) = 0;
};

template <typename ResourceT>
class ResourceHandler: public IResourceHandler {
    friend class ResourceMgr;
public:
    ResourceHandler(std::string name, std::unique_ptr<ResourceT>&& resource, std::shared_ptr<ResourceMgr> resourceMgr):
        m_name(name)
    {
        setNewResource(std::move(resource), resourceMgr);
    }
    ResourceHandler(std::string name): m_markReload(true), m_name(name) {}
    bool ready() const override  { return m_resource != nullptr && m_dependenciesReady; }
    bool updated() const override { return m_updated; }
    ResourceT& resource() { return *m_resource; }
    void update() override
    {
        if (m_dependencies.size() > 0) { // check dependencies first, if there are any dependencies
            if (!m_dependenciesReady) { // dependencies are not ready yet
                bool ready = true;
                for (auto& dependence: m_dependencies)
                    ready &= dependence->ready();
                if (ready) {
                    m_dependenciesReady = true; // now, all dependencies are ready
                    m_updated = true;
                }
            } else { // all dependencies are ready and one of dependencies is updated
                for (auto& dependence: m_dependencies)
                    if (dependence->updated())
                        m_updated = true;
            }
        } else {
            m_dependenciesReady = true;
        }

        if (m_dependenciesReady) { // all dependencies loaded, or there are no dependencies
            if (m_newResource) { // resource was updated
                m_resource = move(m_newResource);
                m_updated = true;
            } else {
                m_updated = false;
            }
        }
    }
    bool markedReload() const override {return m_markReload;}
    void reloadFromSource(std::shared_ptr<ResourceMgr> resourceMgr) override;

private:
    std::unique_ptr<ResourceT> m_newResource = nullptr;
    std::unique_ptr<ResourceT> m_resource = nullptr;
    std::list<std::shared_ptr<IResourceHandler>> m_dependencies;
    bool m_updated = false;
    bool m_dependenciesReady = false;
    bool m_markReload = false;
    std::string m_name;

    void addDependency(std::shared_ptr<IResourceHandler> handler)
    {
        m_dependenciesReady = false;
        m_dependencies.push_back(handler);
    }
    void reload()
    {
        m_markReload = true;
    }
    void load(std::shared_ptr<ResourceMgr> resourceMgr, const std::string& path);

    void setNewResource(std::unique_ptr<ResourceT>&& newResource, std::shared_ptr<ResourceMgr> resourceMgr)
    {
        m_newResource = std::move(newResource);
        m_markReload = false;
        procNewResource(resourceMgr);
    }

    void procNewResource(std::shared_ptr<ResourceMgr> )
    {
        // Do nothing. No default implementation.
    }
};

class ResourceMgr: public Manager<ResourceMgr>, public std::enable_shared_from_this<ResourceMgr>
{
public:
    ResourceMgr(std::shared_ptr<IResourceLoader> resourceLoader):
        m_resourceLoader(resourceLoader)
    {}

    template <typename ResourceT>
    void set(const std::string& path, ResourceT&& resource)
    {
        using namespace std;
        using ResourceTD = typename decay<ResourceT>::type;
        auto newRes = unique_ptr<ResourceTD>(new ResourceTD(forward<ResourceT>(resource)));
        m_resourceMap.emplace(path, make_shared<ResourceHandler<ResourceTD>>(path, std::move(newRes), shared_from_this()));
    }

    template <typename ResourceT>
    std::shared_ptr<ResourceHandler<ResourceT>> get(const std::string& path)
    {
        using namespace std;
        if (m_resourceMap.count(path) == 0)
            m_resourceMap.emplace(path, make_shared<ResourceHandler<ResourceT>>(path));
        return dynamic_pointer_cast<ResourceHandler<ResourceT>>(m_resourceMap[path]);
    }

    void update(TimeDelta) override;

    template <typename Type>
    std::unique_ptr<Type> load(const std::string& path) const
    {
        throw std::runtime_error(path + " Can't be loaded. Template was not specialized for this type of resources.");
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<IResourceHandler>> m_resourceMap;
    std::shared_ptr<IResourceLoader> m_resourceLoader;
};

template <typename ResourceT>
void ResourceHandler<ResourceT>::load(std::shared_ptr<ResourceMgr> resourceMgr, const std::string& path)
{
    m_newResource = resourceMgr->load<ResourceT>(path);
}

template <typename ResourceT>
void ResourceHandler<ResourceT>::reloadFromSource(std::shared_ptr<ResourceMgr> resourceMgr)
{
    setNewResource(std::move(resourceMgr->load<ResourceT>(m_name)), resourceMgr);
}

template <>
void ResourceHandler<Atlas>::procNewResource(std::shared_ptr<ResourceMgr> resourceMgr);

template<>
std::unique_ptr<Texture> ResourceMgr::load<Texture>(const std::string& path) const;

template<>
std::unique_ptr<Atlas> ResourceMgr::load<Atlas>(const std::string& path) const;

template <>
std::shared_ptr<ResourceHandler<Quad>> ResourceMgr::get<Quad>(const std::string& path);

} // flappy

#define RES_MGR MGR<ResourceMgr>()
