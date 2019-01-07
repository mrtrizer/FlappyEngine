#pragma once

#include <memory>

#include <ResInfo.h>

#include "ExecType.h"

namespace flappy {

class ResBase;
class Entity;

/// @addtogroup ResManager
/// @{

class REFLECT IResFactory {
public:
    virtual ~IResFactory() = default;
    virtual std::shared_ptr<ResBase> load(const std::string& resInfo, ExecType execType) = 0;
    virtual std::shared_ptr<ResBase> create(const std::string& name) = 0;
    virtual bool changed(const std::string& name) { return false; }
    virtual void init(std::shared_ptr<ResBase> res) {}
    virtual void deinit(std::shared_ptr<ResBase> res) {}
};

/// @}

} // flappy
