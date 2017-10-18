#pragma once

#include <memory>

#include <Render.h>
#include <ResManager.h>

#include "GLShaderRes.h"
#include "IGLManager.h"

namespace flappy {

/// @brief Holds a shader shared pointer.
/// @details Takes a pointer from static weak pointer.
/// So, it is only one instance of a shader in the program.
/// The instance of a shader program is bound with a class it use.
/// You need to remove all instances of derived class to
/// destroy the instance of GLShaderProgram.
template<typename Derived>
class GLRender: public Render
{
public:
    GLRender()
    {
        addDependency(ResManager<ShaderRes>::id());
        addDependency(IGLManager::id());

        subscribe([this](InitEvent) {
            setShader(manager<ResManager<ShaderRes>>()->getRes("shape_shader", ExecType::ASYNC));
        });
    }

    void setShader(std::shared_ptr<ShaderRes> shaderRes) {
        m_shaderRes = std::dynamic_pointer_cast<GLShaderRes>(shaderRes);
    }
    std::shared_ptr<GLShaderRes> shader() {
        if (m_shaderRes->nextRes() != m_shaderRes)
            m_shaderRes = std::static_pointer_cast<GLShaderRes>(m_shaderRes->lastRes());
        return m_shaderRes;
    }

private:
    std::shared_ptr<GLShaderRes> m_shaderRes;
};

} // flappy
