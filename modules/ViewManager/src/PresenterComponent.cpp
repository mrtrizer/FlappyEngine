#include "PresenterComponent.h"
#include <Tools.h>
#include <View.h>
#include <ViewManager.h>

namespace flappy {

using namespace std;

void PresenterComponent::init() {
    auto viewManager = MGR<ViewManager>();
    if (!viewManager)
        return ERROR_MSG(VOID_VALUE, "ViewManager is not installed.");
    MGR<ViewManager>()->addPresenter(shared_from_this());
    updateView();
}

/// Calls GView::externUpdate if gView is set.
/// The idea is not entirely implemented. This method is
/// called only if frameN is changed in GPresenterSprite.
void PresenterComponent::updateView(){
    if (auto viewPtr = m_view.lock())
        viewPtr->externUpdate(shared_from_this());
}

void PresenterComponent::setColor(const Color& color) {
    m_color = color;
    updateView();
}

} // flappy
