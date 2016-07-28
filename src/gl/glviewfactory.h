#ifndef GLVIEWFACTORY_H
#define GLVIEWFACTORY_H

#include <map>

#include <core/viewfactory.h>

/// @brief GLView factory creates views working with OpenGL.
/// @details A method for texture loading getGLTexture() has to be implemented
/// to work with GViewSprite. But can be stubed and return
/// something like little black square if not used.
/// See specialization of get template function in .cpp.
class GLViewFactory: public ViewFactory {

};

#endif // GLVIEWFACTORY_H
