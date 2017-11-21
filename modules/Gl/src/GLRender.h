#pragma once

#include <memory>

#include <Render.h>
#include <ResManager.h>

#include "GLShaderRes.h"
#include "IGLManager.h"

namespace flappy {

template<typename Derived>
class GLRender: public Render
{
public:
    GLRender();
    void setShader(std::shared_ptr<ShaderRes> shaderRes);
    std::shared_ptr<GLShaderRes> shader();

private:
    std::shared_ptr<GLShaderRes> m_shaderRes;
};

template<typename Derived>
GLRender<Derived>::GLRender()
{
    addDependency(ResManager<ShaderRes>::id());
    addDependency(IGLManager::id());

    subscribe([this](InitEvent) {
        setShader(manager<ResManager<ShaderRes>>()->getRes("shape_shader", ExecType::ASYNC));
    });
}

template<typename Derived>
void GLRender<Derived>::setShader(std::shared_ptr<ShaderRes> shaderRes) {
    m_shaderRes = std::dynamic_pointer_cast<GLShaderRes>(shaderRes);
}

template<typename Derived>
std::shared_ptr<GLShaderRes> GLRender<Derived>::shader() {
    if (m_shaderRes->resUpdated())
        m_shaderRes = std::static_pointer_cast<GLShaderRes>(m_shaderRes->lastRes());
    return m_shaderRes;
}

} // flappy
