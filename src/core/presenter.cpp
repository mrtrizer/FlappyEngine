#include "presenter.h"
#include "viewfactory.h"
#include "view.h"
#include "tools.h"

/// Returns GView instance and creates new if gView is nullptr.
std::shared_ptr<View> Presenter::getGView(const ViewFactory &factory) {
    if (gView == nullptr)
        gView = makeGView(factory);
    return gView;
}

/// Clear a shared pointer to GView instance. Used by GWorldView destructor.
/// shared pointer will reinitilized in next getGView call.
void Presenter::cleanGView(){
    gView = nullptr;
}

/// Calls GView::externUpdate if gView is set.
/// The idea is not entirely implemented. This method is
/// called only if frameN is changed in GPresenterSprite.
void Presenter::updateView(){
    if (gView != nullptr)
        gView->externUpdate(shared_from_this());
}

/// Changes current frame pointer and informs GView about it.
void Sprite::setFrameN(int frameN) {
    this->frameN = frameN;
    updateView();
}

std::shared_ptr<View> Sprite::makeGView(const ViewFactory &factory) {
    return factory.getGViewSprite(*this);
}

std::shared_ptr<View> CircleShape::makeGView(const ViewFactory &factory) {
    return factory.getGViewCircle(*this);
}

std::shared_ptr<View> RectShape::makeGView(const ViewFactory &factory) {
    return factory.getGViewRect(*this);
}
