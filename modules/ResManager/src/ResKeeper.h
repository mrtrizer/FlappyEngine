#pragma once

#include <memory>

#include <SafePtr.h>

#include "ResInfo.h"

namespace flappy {

class Res;
class IResFactory;
class Entity;

/// @addtogroup ResManager
/// @{

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
    void updateRes(SafePtr<IResFactory>, const ResInfo& resInfo, SafePtr<Entity> entity);
    bool dependencyChanged() const;
    std::shared_ptr<Res> actualRes();

private:
    bool m_changed;
    std::shared_ptr<Res> m_res;
};

/// @}

} // flappy
