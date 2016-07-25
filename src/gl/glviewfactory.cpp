#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>

#include "glviewfactory.h"
#include "glviewshape.h"
#include "glviewsprite.h"

template <> std::shared_ptr<View> ViewFactory::get<Sprite>(const Sprite& presenter) const {
    auto & presenterSprite = dynamic_cast<const Sprite &>(presenter);
    std::shared_ptr<Texture> texture;
    auto texturePath = presenterSprite.getPath();
    auto mapIter = textureMap.find(texturePath);
    if (mapIter == textureMap.end()) {
        texture = getTexture(presenterSprite.getPath());
        textureMap[texturePath] = texture;
    } else {
        texture = mapIter->second;
    }
    return std::make_shared<GLViewSprite>(texture,presenterSprite);
}

template <> std::shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape& presenter) const {
    auto & presenterCircle = dynamic_cast<const CircleShape &>(presenter);
    return std::make_shared<GViewCircle>(CIRCLE_VERTEX_CNT, presenterCircle.getR_());
}

template <> std::shared_ptr<View> ViewFactory::get<RectShape>(const RectShape& presenter) const {
    auto & presenterRect = dynamic_cast<const RectShape &>(presenter);
    return std::make_shared<GViewRect>(presenterRect.getWidth(), presenterRect.getHeight());
}
