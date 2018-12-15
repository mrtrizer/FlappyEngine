#pragma once

#include <memory>

#include <Handle.hpp>

#include "ResInfo.h"
#include "ExecType.h"

namespace flappy {

class ResBase;
class Entity;
class IFileMonitorManager;
class IResFactory;

/// @addtogroup ResManager
/// @{

class ResKeeper
{
public:
    ResKeeper(Handle<IResFactory> resFactory, std::string name);
    virtual ~ResKeeper() = default;
    ResKeeper(const ResKeeper&) = delete;
    ResKeeper& operator=(const ResKeeper&) & = delete;
    ResKeeper(ResKeeper&&) = default;
    ResKeeper& operator=(ResKeeper&&) & = default;

    bool needRemove();
    void cleanUpRes();
    void updateRes();
    bool dependencyChanged();
    std::shared_ptr<ResBase> actualRes(ExecType execType);

private:
    Handle<IResFactory> m_resFactory;
    std::string m_name;
    std::shared_ptr<ResBase> m_res;
    bool m_loaded = false;
};

/// @}

} // flappy
