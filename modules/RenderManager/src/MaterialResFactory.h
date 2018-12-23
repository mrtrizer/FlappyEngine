#pragma once

#include <string>

#include <Handle.hpp>

#include "ResFactory.h"
#include "MaterialRes.h"

namespace flappy {

class IFileMonitorManager;
class Entity;

class MaterialResFactory : public ResFactory<MaterialRes>
{
public:
    MaterialResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
    
    Handle<ResManager<JsonRes>> m_jsonResManager;
    Handle<ResManager<TextureRes>> m_textureResManager;
    Handle<ResManager<ShaderRes>> m_shaderResManager;
};

} // flappy
