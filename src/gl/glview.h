#pragma once

#include <memory>

#include "core/view.h"
#include "glshaderprogram.h"

namespace flappy {

using namespace std;

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
        //TODO: move to getShader for garaties of calling in GL context?
        if (weakShader.expired())
            weakShader = shader = make_shared<GLShaderProgram>(vSource, fSource);
        else
            shader = weakShader.lock();
    }
protected:
    shared_ptr<GLShaderProgram> getShader(){return shader;}
private:
    static weak_ptr<GLShaderProgram> weakShader;
    shared_ptr<GLShaderProgram> shader;
};

template<typename Derived>
weak_ptr<GLShaderProgram> GLView<Derived>::weakShader;

} // flappy
