#include <core/sprite.h>
#include <core/rectshape.h>
#include <core/circleshape.h>
#include <core/resourcemanager.h>

#include "glviewfactory.h"
#include "glviewrect.h"
#include "glviewcircle.h"
#include "glviewsprite.h"

namespace flappy {

using namespace std;

template <> shared_ptr<View> ViewFactory::get<Sprite>(const Sprite& presenterSprite) const {
    return make_shared<GLViewSprite>(presenterSprite);
}

template <> shared_ptr<View> ViewFactory::get<CircleShape>(const CircleShape&) const {
    return make_shared<GViewCircle>(m_circleVectexCnt);
}

template <> shared_ptr<View> ViewFactory::get<RectShape>(const RectShape&) const {
    return make_shared<GLViewRect>();
}

} // flappy
