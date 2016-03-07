#include <cmath>
#include <functional>

#include "gtools.h"
#include "gcollider.h"
#include "gobj.h"

/// Common tools for model
namespace GTools {
using namespace std;

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

/// Done used as exception object when found
/// appropriate intersection method and contains
/// intersect checking result.
/// @see check()
class Done {
public:
    Done(bool result): result(result){}
    inline bool getResult() const {return result;}
private:
    bool result;
};

/// Template function is used for calling of appropriate
/// collision checking method.
/// @see isIntersect()
template <typename GColliderT1, typename GColliderT2>
static void check(const GObj &gObj1, const GObj &gObj2) {
    try { //is it valid cast?
    auto gColliderT1 = dynamic_cast<const GColliderT1 &>(gObj1);
    auto gColliderT2 = dynamic_cast<const GColliderT2 &>(gObj2);
    throw Done(isIntersect(gColliderT1, gColliderT2, gObj1, gObj2)); //allright, return checking result
    } catch (std::bad_cast &) { //if bad cast, try to reverse and over
        try {
        auto gColliderT1 = dynamic_cast<const GColliderT2 &>(gObj1);
        auto gColliderT2 = dynamic_cast<const GColliderT1 &>(gObj2);
        throw Done(isIntersect(gColliderT1, gColliderT2, gObj2, gObj1)); //allright, return checking result
        } catch (std::bad_cast &) {} //we can't cast, ignore checking
    }
}

bool isIntersect(const GObj &gObj1, const GObj &gObj2) {
    try { //if checking done, catch Done exception
        check<GColliderRect, GColliderCircle>(gObj1, gObj2);
        check<GColliderCircle, GColliderCircle>(gObj1, gObj2);
        check<GColliderRect, GColliderRect>(gObj1, gObj2);
    } catch (Done & done) {
        return done.getResult();
    }
    return false;
}

//http://stackoverflow.com/questions/108318/whats-the-simplest-way-to-test-whether-a-number-is-a-power-of-2-in-c
bool isPowOfTwo(int x) {
    return !(x == 0) && !(x & (x - 1));
}

}
