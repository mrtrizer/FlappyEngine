#include "glviewfactory.h"
#include "glviewshape.h"
#include "glviewsprite.h"

GViewFactory::GViewP GLViewFactory::getGViewSprite(const GPresenter &presenter) const {
    auto & presenterSprite = dynamic_cast<const GPresenterSprite &>(presenter);
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

GViewFactory::GViewP GLViewFactory::getGViewCircle(const GPresenter &presenter) const {
    auto & presenterCircle = dynamic_cast<const GPresenterCircle &>(presenter);
    return std::make_shared<GViewCircle>(CIRCLE_VERTEX_CNT, presenterCircle.getR_());
}

GViewFactory::GViewP GLViewFactory::getGViewRect(const GPresenter &presenter) const {
    auto & presenterRect = dynamic_cast<const GPresenterRect &>(presenter);
    return std::make_shared<GViewRect>(presenterRect.getWidth(), presenterRect.getHeight());
}
