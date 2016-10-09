#include "presenter.h"
#include "view.h"
#include "tools.h"
#include "viewmanager.h"

namespace flappy {

using namespace std;

void Presenter::init() {
    auto viewManager = MGR<ViewManager>();
    if (!viewManager)
        return ERROR_MSG(VOID_VALUE, "ViewManager is not installed.");
    MGR<ViewManager>()->addPresenter(shared_from_this());
    updateView();
}

/// Calls GView::externUpdate if gView is set.
/// The idea is not entirely implemented. This method is
/// called only if frameN is changed in GPresenterSprite.
void Presenter::updateView(){
    if (auto viewPtr = m_view.lock())
        viewPtr->externUpdate(shared_from_this());
}

void Presenter::setColor(const Color& color) {
    m_color = color;
    updateView();
}

} // flappy
