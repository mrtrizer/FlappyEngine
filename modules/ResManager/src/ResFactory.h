#pragma once

#include <memory>

namespace flappy {

class ResManager;
class Res;

/// @addtogroup ResManager
/// @{

/// Base class for all resource factories.
/// @details Every automaticly loaded resource should have
/// factory. Factories are used to create resource instacies
/// different ways on different platforms.
/// Factory has create() method to create default resource
/// instace right on first user request.
/// load() method should provide synchronious loading of
/// requested resource. It's usially called in a separate thread.
/// changed method returns true if resource was changed since
/// last load.
class ResFactory {
public:
    ResFactory() = default;
    virtual ~ResFactory() = default;
    ResFactory(const ResFactory&) = default;
    ResFactory& operator=(const ResFactory&) & = default;
    ResFactory(ResFactory&&) = default;
    ResFactory& operator=(ResFactory&&) & = default;

    virtual std::shared_ptr<Res> load(const std::string&, std::shared_ptr<ResManager>) = 0;
    virtual std::shared_ptr<Res> create(const std::string& name, std::shared_ptr<ResManager> resManager) {
        return load(name, resManager);
    }

    virtual bool changed(const std::string&) {
        return false;
    }
};

/// @}

} // flappy
