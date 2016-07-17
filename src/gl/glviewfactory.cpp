#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>

#include "glviewfactory.h"
#include "glviewshape.h"
#include "glviewsprite.h"

ViewFactory::GViewP GLViewFactory::getGViewSprite(const Presenter &presenter) const {
    auto & presenterSprite = dynamic_cast<const Sprite &>(presenter);
    std::shared_ptr<GLTexture> texture;
    auto texturePath = presenterSprite.getPath();
    auto mapIter = textureMap.find(texturePath);
    if (mapIter == textureMap.end()) {
        texture = getGLTexture(presenterSprite.getPath());
        textureMap[texturePath] = texture;
    } else {
        texture = mapIter->second;
    }
    return std::make_shared<GLViewSprite>(texture,presenterSprite);
}

ViewFactory::GViewP GLViewFactory::getGViewCircle(const Presenter &presenter) const {
    auto & presenterCircle = dynamic_cast<const CircleShape &>(presenter);
    return std::make_shared<GViewCircle>(CIRCLE_VERTEX_CNT, presenterCircle.getR_());
}

ViewFactory::GViewP GLViewFactory::getGViewRect(const Presenter &presenter) const {
    auto & presenterRect = dynamic_cast<const RectShape &>(presenter);
    return std::make_shared<GViewRect>(presenterRect.getWidth(), presenterRect.getHeight());
}
