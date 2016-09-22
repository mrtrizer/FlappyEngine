#pragma once

#include <memory>
#include <list>
#include <string>

namespace flappy {

class ResManager;
class Atlas;

class IResHandler {
public:
    virtual ~IResHandler() = default;
    virtual void update() = 0;
    virtual bool ready() const = 0;
    virtual bool updated() const = 0;
    virtual bool markedReload() const = 0;
    virtual void reloadFromSource(std::shared_ptr<ResManager> resManager) = 0;
};

template <typename ResT>
class ResHandler: public IResHandler {
    friend class ResManager;
public:
    ResHandler(std::string name, std::unique_ptr<ResT>&& resource, std::shared_ptr<ResManager> resManager):
        m_name(name)
    {
        setNewResource(std::move(resource), resManager);
    }
    ResHandler(std::string name): m_markReload(true), m_name(name) {}
    bool ready() const override  { return m_resource != nullptr && m_dependenciesReady; }
    bool updated() const override { return m_updated; }
    ResT& resource() { return *m_resource; }
    void update() override
    {
        if (!m_dependencies.empty()) { // check dependencies first, if there are any dependencies
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

    void reloadFromSource(std::shared_ptr<ResManager> resManager) override {
        // ResManager is incomplete class becouse of dependency loop
        // but I also can't move the method to .cpp becouse of template class
        [this](auto autoResManager){
            setNewResource(std::move(autoResManager->template load<ResT>(m_name)), autoResManager);
        }(resManager);
    }

private:
    std::unique_ptr<ResT> m_newResource = nullptr;
    std::unique_ptr<ResT> m_resource = nullptr;
    std::list<std::shared_ptr<IResHandler>> m_dependencies;
    bool m_updated = false;
    bool m_dependenciesReady = false;
    bool m_markReload = false;
    std::string m_name;

    void addDependency(std::shared_ptr<IResHandler> handler)
    {
        m_dependenciesReady = false;
        m_dependencies.push_back(handler);
    }
    void reload()
    {
        m_markReload = true;
    }

    void setNewResource(std::unique_ptr<ResT>&& newResource, std::shared_ptr<ResManager> resManager)
    {
        m_newResource = std::move(newResource);
        m_markReload = false;
        procNewResource(resManager);
    }

    /// No default implementation. Do not remove.
    void procNewResource(std::shared_ptr<ResManager> ) {}

};

template <>
void ResHandler<Atlas>::procNewResource(std::shared_ptr<ResManager> resManager);


} // flappy
