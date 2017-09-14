#pragma once

#include <memory>

#include <View.h>
#include <ResManager.h>
#include "GLShaderProgram.h"

namespace flappy {

// TODO: Right now this class became useless.

/// @brief Holds a shader shared pointer.
/// @details Takes a pointer from static weak pointer.
/// So, it is only one instance of a shader in the program.
/// The instance of a shader program is bound with a class it use.
/// You need to remove all instances of derived class to
/// destroy the instance of GLShaderProgram.
template<typename Derived>
class GLView: public View
{
public:
    GLView()
    {
        addDependency(ResManager<GLShaderProgram>::id());

        subscribe([this](InitEvent) {
            setShader(manager<ResManager<GLShaderProgram>>()->getRes("shape_shader"));
        });
    }

    void setShader(std::shared_ptr<GLShaderProgram> shaderRes) { m_shaderRes = shaderRes; }
    std::shared_ptr<GLShaderProgram> shader() {
        m_shaderRes = std::static_pointer_cast<GLShaderProgram>(m_shaderRes->lastRes());
        return m_shaderRes;
    }

private:
    std::shared_ptr<GLShaderProgram> m_shaderRes;
};

} // flappy
