#pragma once

#include <memory>

namespace flappy {

class ResManager;
class IRes;
class IResHandler;
class ResManager;

class IResFactory {
public:
    /// Implementation should perform synchonious loading of resource.
    /// @details This function will called in resource load thread. It's expected
    /// a shared pointer to completely intialized resource, or nullptr as resoult.
    virtual std::shared_ptr<IRes> load(const std::string&) { return nullptr; }

    /// Implementation should perform initialization of handler, if needed.
    /// @details A handler initialization will be performed in
    /// ResourceManager::get() method
    virtual void initHandler(std::shared_ptr<IResHandler>, std::shared_ptr<ResManager>) {}

    // The function is called with cmpletely initialized resource. So, this method could be moved to IRes.
    // I think, there are ways to get rid from this method at all.
    /// Implementation should update dependencies of handler or other synchronization of
    /// resourece handler and resource instance itself.
    virtual void updateHandler(std::shared_ptr<IResHandler>, std::shared_ptr<IRes>, std::shared_ptr<ResManager>) {}
};

} // flappy
