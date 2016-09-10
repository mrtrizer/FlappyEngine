#include "rectshape.h"
#include "viewfactory.h"

namespace flappy {

using namespace std;

shared_ptr<View> RectShape::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

void RectShape::setColor(const Color& color) {
    m_color = color;
    updateView();
}

} // flappy
