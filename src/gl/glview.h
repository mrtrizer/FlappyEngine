#pragma once

#include <memory>

#include "core/view.h"
#include "glshaderprogram.h"

namespace flappy {

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
    GLView(const char * vSource, const char * fSource) {
        using namespace std;
        //TODO: move to getShader for garaties of calling in GL context?
        if (m_weakShader.expired())
            m_weakShader = m_shader = make_shared<GLShaderProgram>(vSource, fSource);
        else
            m_shader = m_weakShader.lock();
    }
protected:
    std::shared_ptr<GLShaderProgram> getShader(){return m_shader;}
private:
    static std::weak_ptr<GLShaderProgram> m_weakShader;
    std::shared_ptr<GLShaderProgram> m_shader;
};

template<typename Derived>
std::weak_ptr<GLShaderProgram> GLView<Derived>::m_weakShader;

} // flappy
