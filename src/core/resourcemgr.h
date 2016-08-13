#pragma once

#include <unordered_map>

#include <core/manager.h>

namespace flappy {

class Texture;
class Atlas;

class IResourceHandler {
public:
    virtual ~IResourceHandler() noexcept {}
    virtual void update() = 0;
};

template <typename ResourceT>
class ResourceHandler: public IResourceHandler {
public:
    ResourceHandler(std::unique_ptr<ResourceT>&& resource):m_newResource(move(resource)) {}
    bool ready() { return m_resource != nullptr; }
    bool updated() { return m_updated; }
    ResourceT& resource() { return *m_resource; }
    void update() override {
        if (m_newResource) {
            m_resource = move(m_newResource);
            m_updated = true;
        } else {
            m_updated = false;
        }
    }
    void setNewResource(std::unique_ptr<ResourceT> newResource) { m_newResource = newResource; }
private:
    std::unique_ptr<ResourceT> m_newResource;
    std::unique_ptr<ResourceT> m_resource = nullptr;
    bool m_updated = false;
};

class ResourceMgr: public Manager<ResourceMgr>
{
public:
    template <typename ResourceT>
    void set(const std::string& path, ResourceT&& resource) {
        using namespace std;
        m_resourceMap[path] = ResourceHandler<ResourceT>(forward<ResourceT>(resource));
    }

    template <typename ResourceT>
    void load(const std::string& path) {
        using namespace std;
        if (m_resourceMap.count(path) == 0)
            m_resourceMap[path] = make_shared<ResourceHandler<ResourceT>>(move(loadRes<ResourceT>(path)));
    }

    template <typename ResourceT>
    std::shared_ptr<ResourceHandler<ResourceT>> get(const std::string& path) {
        using namespace std;
        load<ResourceT>(path);
        return dynamic_pointer_cast<ResourceHandler<ResourceT>>(m_resourceMap[path]);
    }

    void update(TimeDelta) override {
        for (auto& resPair: m_resourceMap)
            resPair.second->update();
    }

public:
    std::unordered_map<std::string, std::shared_ptr<IResourceHandler>> m_resourceMap;

    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const = 0;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const = 0;

    template <typename Type>
    std::unique_ptr<Type> loadRes(const std::string& path) const {
        throw std::runtime_error(path + " Can't be loaded. Template was not specialized for this type of resources.");
        return nullptr; }
};

template<> std::unique_ptr<Texture> ResourceMgr::loadRes<Texture>(const std::string& path) const;
template<> std::unique_ptr<Atlas> ResourceMgr::loadRes<Atlas>(const std::string& path) const;

} // flappy
