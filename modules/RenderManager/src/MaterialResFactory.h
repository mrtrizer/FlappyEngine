#pragma once

#include <string>

#include "ResFactory.h"
#include "MaterialRes.h"

namespace flappy {

class IFileMonitorManager;
class Entity;

class MaterialResFactory : public ResFactory<MaterialRes>
{
public:
    MaterialResFactory();

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
};

} // flappy
