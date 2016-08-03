#include "circleshape.h"

#include "viewfactory.h"

namespace flappy {

shared_ptr<View> CircleShape::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

} // flappy
