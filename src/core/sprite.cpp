#include "sprite.h"
#include "viewfactory.h"

namespace flappy {

using namespace std;

shared_ptr<View> Sprite::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

} // flappy
