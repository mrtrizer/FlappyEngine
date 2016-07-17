#include "rectshape.h"
#include "viewfactory.h"

std::shared_ptr<View> RectShape::makeGView(const ViewFactory &factory) {
    return factory.getGViewRect(*this);
}
