#include "rectshape.h"
#include "viewfactory.h"

namespace flappy {

using namespace std;

shared_ptr<View> RectShape::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

} // flappy
