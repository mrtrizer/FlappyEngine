#pragma once

#include <memory>

#include <SafePtr.h>

#include "ResInfo.h"

namespace flappy {

class Res;
class Entity;
class IFileMonitorManager;
class IResFactory;

/// @addtogroup ResManager
/// @{

class ResKeeper
{
public:
    ResKeeper(SafePtr<flappy::IResFactory> resFactory, std::string name);
    virtual ~ResKeeper() = default;
    ResKeeper(const ResKeeper&) = delete;
    ResKeeper& operator=(const ResKeeper&) & = delete;
    ResKeeper(ResKeeper&&) = default;
    ResKeeper& operator=(ResKeeper&&) & = default;

    void pushRes(std::shared_ptr<Res> res);
    bool needRemove();
    void cleanUpRes();
    void updateRes();
    bool dependencyChanged();
    std::shared_ptr<Res> actualRes();

private:
    SafePtr<IResFactory> m_resFactory;
    std::string m_name;
    std::shared_ptr<Res> m_res;
    bool m_loaded = false;
};

/// @}

} // flappy
