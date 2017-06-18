#include "View.h"

#include <ViewManager.h>

namespace flappy {

using namespace glm;
using namespace std;

View::View() {
    events()->subscribeIn([this](OnManagerAdded e) {
        if (e.castTo<ViewManager>() != nullptr)
            manager<ViewManager>()->registerRenderElement(selfPointer<View>());
    });
}

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void View::redraw(const mat4 & pMartrix, const mat4 & mvMatrix) {
    draw(pMartrix, mvMatrix);
}

} // flappy
