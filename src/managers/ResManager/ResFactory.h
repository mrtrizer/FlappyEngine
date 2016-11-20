#pragma once

#include <memory>

namespace flappy {

class ResManager;
class Res;

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

} // flappy
