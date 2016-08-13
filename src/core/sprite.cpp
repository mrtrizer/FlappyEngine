#include "sprite.h"
#include "viewfactory.h"

namespace flappy {

using namespace std;

/// Changes current frame pointer and informs GView about it.
void Sprite::setFrameN(int frameN) {
    this->m_frameN = frameN;
    updateView();
}

shared_ptr<View> Sprite::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}

} // flappy
