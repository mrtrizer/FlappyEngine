#ifndef GLVIEWFACTORY_H
#define GLVIEWFACTORY_H

#include <map>

#include <core/viewfactory.h>

class GLTexture;

/// @brief GLView factory creates views working with OpenGL.
/// @details A method for texture loading getGLTexture() has to be implemented
/// to work with GViewSprite. But can be stubed and return
/// something like little black square if not used.
class GLViewFactory: public ViewFactory {
public:
    virtual ~GLViewFactory() {}
    virtual GViewP getGViewSprite(const Presenter & presenter) const override;
    virtual GViewP getGViewCircle(const Presenter & presenter) const override;
    virtual GViewP getGViewRect(const Presenter & presenter) const override;
    virtual std::shared_ptr<GLTexture> getGLTexture(std::string path) const = 0;
private:
    typedef std::map<std::string,std::shared_ptr<GLTexture>> TextureMap;

    const int CIRCLE_VERTEX_CNT = 30;
    mutable TextureMap textureMap;
};

#endif // GLVIEWFACTORY_H
