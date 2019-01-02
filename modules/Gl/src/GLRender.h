#pragma once

#include <memory>

#include <Render.h>
#include <ResManager.h>

#include "GLShaderRes.h"
#include "IGLManager.h"

namespace flappy {

class IGLManager;
    
class GLRender: public Render
{
public:
    GLRender(Handle<Entity> entity)
        : Render(entity)
        , m_shaderResManager(entity->hierarchy()->manager<ResManager<ShaderRes>>())
        , m_glManager(entity->hierarchy()->manager<IGLManager>())
    {
        setShader(m_shaderResManager->getRes("shape_shader", ExecType::ASYNC));
    }
    
    void setShader(std::shared_ptr<ShaderRes> shaderRes) {
        m_shaderRes = std::dynamic_pointer_cast<GLShaderRes>(shaderRes);
    }
    std::shared_ptr<GLShaderRes> shader() {
        if (m_shaderRes->resUpdated())
            m_shaderRes = std::static_pointer_cast<GLShaderRes>(m_shaderRes->lastRes());
        return m_shaderRes;
    }

private:
    std::shared_ptr<GLShaderRes> m_shaderRes;
    Handle<ResManager<ShaderRes>> m_shaderResManager;
    Handle<IGLManager> m_glManager;
};

} // flappy
