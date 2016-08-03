#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>
#include <core/resourcemgr.h>

#include "glviewfactory.h"
#include "glviewrect.h"
#include "glviewcircle.h"
#include "glviewsprite.h"

namespace flappy {

template <> shared_ptr<View> ViewFactory::get<Sprite>(const Sprite& presenterSprite) const {
    shared_ptr<Texture> texture;
    auto texturePath = presenterSprite.path();
    auto mapIter = m_textureMap.find(texturePath);
    if (mapIter == m_textureMap.end()) {
        texture = FlappyApp::inst().resourceMgr()->getTexture(presenterSprite.path());
        m_textureMap[texturePath] = texture;
    } else {
        texture = mapIter->second;
    }
    return make_shared<GLViewSprite>(texture,presenterSprite);
}

template <> shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape& presenterCircle) const {
    return make_shared<GViewCircle>(m_circleVectexCnt, presenterCircle.width());
}

template <> shared_ptr<View> ViewFactory::get<RectShape>(const RectShape& presenterRect) const {
    return make_shared<GViewRect>(presenterRect.width(), presenterRect.height());
}

} // flappy
