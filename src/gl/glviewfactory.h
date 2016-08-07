#pragma once

#include <map>

#include <core/viewfactory.h>

namespace flappy {

/// @brief GLView factory creates views working with OpenGL.
/// @details A method for texture loading getGLTexture() has to be implemented
/// to work with GViewSprite. But can be stubed and return
/// something like little black square if not used.
/// See specialization of get template function in .cpp.
class GLViewFactory: public ViewFactory {
public:
    using ViewFactory::ViewFactory;
};

} // flappy
