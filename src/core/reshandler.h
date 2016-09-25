#pragma once

#include <memory>
#include <list>
#include <string>

#include "tools.h"
#include "res.h"

namespace flappy {

class Atlas;

class IResHandler {
public:
    IResHandler() = default;
    IResHandler(const IResHandler&) = delete;
    IResHandler(IResHandler&&) = default;
    virtual ~IResHandler() = default;
    IResHandler& operator=(const IResHandler&) = delete;
    IResHandler& operator=(IResHandler&&) = default;

    virtual void update() = 0;
    virtual bool ready() const = 0;
    virtual bool updated() const = 0;
    virtual bool loading() const = 0;
    virtual void setNewResource(std::shared_ptr<IRes>) = 0;
    virtual std::string path() const = 0;
    virtual void addDependency(std::shared_ptr<IResHandler> handler) = 0;
};

template <typename ResT>
class ResHandler final: public IResHandler {
    friend class ResManager;
public:
    ResHandler(const std::string& name): m_loading(true), m_path(name) {}

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

    void setNewResource(std::shared_ptr<IRes> newResource) override
    {
        if (newResource->id() != ClassId<IRes, ResT>::id())
            return ERROR_MSG(VOID_VALUE, "Resource type mismatch");
        m_newResource = std::dynamic_pointer_cast<ResT>(newResource);
        m_loading = false;
        m_dependencies.clear();
    }

    void addDependency(std::shared_ptr<IResHandler> handler) override
    {
        m_dependenciesReady = false;
        m_dependencies.push_back(handler);
    }

    bool ready() const override  { return m_resource != nullptr && m_dependenciesReady; }

    bool updated() const override { return m_updated; }

    std::shared_ptr<ResT> resource() { return m_resource; }

    bool loading() const override {return m_loading;}

    std::string path() const override { return m_path; }

    unsigned id() const { return ClassId<IRes, ResT>::id(); }

    void reload() { m_loading = true; }

private:
    std::shared_ptr<ResT> m_newResource = nullptr;
    std::shared_ptr<ResT> m_resource = nullptr;
    std::list<std::shared_ptr<IResHandler>> m_dependencies;
    bool m_updated = false;
    bool m_dependenciesReady = false;
    bool m_loading = false;
    std::string m_path;
};

} // flappy
