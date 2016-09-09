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
    return make_shared<GLViewSprite>(presenterSprite);
}

template <> shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape& presenterCircle) const {
    return make_shared<GViewCircle>(m_circleVectexCnt, presenterCircle.size().x);
}

template <> shared_ptr<View> ViewFactory::get<RectShape>(const RectShape& presenterRect) const {
    return make_shared<GViewRect>(presenterRect.size().x, presenterRect.size().y);
}

} // flappy
