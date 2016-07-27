#include "sprite.h"
#include "viewfactory.h"

/// Changes current frame pointer and informs GView about it.
void Sprite::setFrameN(int frameN) {
    this->m_frameN = frameN;
    updateView();
}

std::shared_ptr<View> Sprite::makeGView(const ViewFactory &factory) {
    return factory.get(*this);
}
