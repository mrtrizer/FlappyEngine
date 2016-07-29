#include "circleshape.h"

#include "viewfactory.h"

std::shared_ptr<View> CircleShape::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}
