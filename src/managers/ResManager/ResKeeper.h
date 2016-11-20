#pragma once

#include <memory>

namespace flappy {

class Res;
class ResFactory;
class ResManager;

class ResKeeper
{
public:
    ResKeeper(std::shared_ptr<Res> res, bool changed = true);
    virtual ~ResKeeper() = default;
    ResKeeper(const ResKeeper&) = delete;
    ResKeeper& operator=(const ResKeeper&) & = delete;
    ResKeeper(ResKeeper&&) = default;
    ResKeeper& operator=(ResKeeper&&) & = default;

    bool needRemove();
    void cleanUpRes();
    void updateRes(std::shared_ptr<ResFactory>, std::string name, std::shared_ptr<ResManager>);
    bool dependencyChanged() const;
    std::shared_ptr<Res> actualRes();

private:
    bool m_changed;
    std::shared_ptr<Res> m_res;
};

} // flappy
