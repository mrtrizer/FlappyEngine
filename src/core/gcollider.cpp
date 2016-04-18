#include "gcollider.h"


/// Intersect checking for two circles
bool isIntersect(const GColliderCircle & gCollider1, const GColliderCircle & gCollider2, const GObj &gObj1, const GObj &gObj2) {
    auto pos1 = gObj1.getAPos();
    auto pos2 = gObj2.getAPos();
    double dX = pos1.getX() - pos2.getX();
    double dY = pos1.getY() - pos2.getY();
    int sumR = gCollider1.getR() + gCollider2.getR();
    if (sumR * sumR  > (dX * dX + dY * dY)) //radius cubed instead sqrt(dist)
        return true;
    return false;
}

/// Intersect checking for circle and rect
bool isIntersect(const GColliderRect & rect, const GColliderCircle & circle, const GObj & gObj1, const GObj & gObj2) {
    //http://yal.cc/rectangle-circle-intersection-test/
    auto r = circle.getR();
    auto width = rect.getWidth();
    auto height = rect.getHeight();
    auto ciclePos = gObj2.getAPos();
    auto rectPos = gObj1.getAPos();
    double dX = ciclePos.getX() - std::max(rectPos.getX(), std::min(ciclePos.getX(), rectPos.getX() + width));
    double dY = ciclePos.getY() - std::max(rectPos.getY(), std::min(ciclePos.getY(), rectPos.getY() + height));
    return (dX * dX + dY * dY) < (r * r);
}

bool isIntersect(const GColliderCircle & circle, const GColliderRect & rect,const GObj & gObj1, const GObj & gObj2) {
    return isIntersect(rect, circle, gObj1, gObj2);
}

/// Intersect checking for two rects. Not implemented yet.
bool isIntersect(const GColliderRect &, const GColliderRect &, const GObj &, const GObj &) {
    return false;
}

/// Template function is used for calling of appropriate
/// collision checking method.
/// @see isIntersect()
template <typename GColliderT1, typename GColliderT2>
static bool check(const GObj &gObj1, const GObj &gObj2) {
    auto gColliderT1 = dynamic_cast<const GColliderT1 *>(&gObj1);
    if (gColliderT1 != nullptr) {
        auto gColliderT2 = dynamic_cast<const GColliderT2 *>(&gObj2);
        if (gColliderT2 != nullptr)
            return isIntersect(*gColliderT1, *gColliderT2, gObj1, gObj2);
    }
    gColliderT1 = dynamic_cast<const GColliderT1 *>(&gObj2);
    if (gColliderT1 != nullptr) {
        auto gColliderT2 = dynamic_cast<const GColliderT2 *>(&gObj1);
        if (gColliderT2 != nullptr)
            return isIntersect(*gColliderT1, *gColliderT2, gObj2, gObj1);
    }
    return false;
}

bool GCollider::isIntersect(const GObj &gObj1, const GObj &gObj2) {
    bool result = false;
    result |= check<GColliderRect, GColliderCircle>(gObj1, gObj2);
    result |=check<GColliderCircle, GColliderCircle>(gObj1, gObj2);
    result |=check<GColliderRect, GColliderRect>(gObj1, gObj2);
    return result;
}

