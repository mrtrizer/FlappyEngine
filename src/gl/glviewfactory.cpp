#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>
#include <core/resourcemgr.h>

#include "glviewfactory.h"
#include "glviewrect.h"
#include "glviewcircle.h"
#include "glviewsprite.h"

namespace flappy {

using namespace std;

template <> shared_ptr<View> ViewFactory::get<Sprite>(const Sprite& presenterSprite) const {
    auto texturePath = presenterSprite.path();
    bool isImgPath = true;
    for (char& c: texturePath)
        if (c == ':')
            isImgPath = false;
    if (isImgPath)
        texturePath = string(":") + texturePath;
    auto texture = flappyApp().lock()->MGR<ResourceMgr>()->get<Quad>(texturePath);
    return make_shared<GLViewSprite>(texture,presenterSprite);
}

template <> shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape& presenterCircle) const {
    return make_shared<GViewCircle>(m_circleVectexCnt, presenterCircle.width());
}

template <> shared_ptr<View> ViewFactory::get<RectShape>(const RectShape& presenterRect) const {
    return make_shared<GViewRect>(presenterRect.width(), presenterRect.height());
}

} // flappy
