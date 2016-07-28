#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>

#include "glviewfactory.h"
#include "glviewrect.h"
#include "glviewcircle.h"
#include "glviewsprite.h"

template <> std::shared_ptr<View> ViewFactory::get<Sprite>(const Sprite& presenterSprite) const {
    std::shared_ptr<Texture> texture;
    auto texturePath = presenterSprite.path();
    auto mapIter = m_textureMap.find(texturePath);
    if (mapIter == m_textureMap.end()) {
        texture = getTexture(presenterSprite.path());
        m_textureMap[texturePath] = texture;
    } else {
        texture = mapIter->second;
    }
    return std::make_shared<GLViewSprite>(texture,presenterSprite);
}

template <> std::shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape& presenterCircle) const {
    return std::make_shared<GViewCircle>(CIRCLE_VERTEX_CNT, presenterCircle.width());
}

template <> std::shared_ptr<View> ViewFactory::get<RectShape>(const RectShape& presenterRect) const {
    return std::make_shared<GViewRect>(presenterRect.width(), presenterRect.height());
}
