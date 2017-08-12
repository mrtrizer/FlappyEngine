#include "View.h"

#include <ViewManager.h>
#include <Tools.h>

namespace flappy {

using namespace glm;
using namespace std;

View::View(TypeIdList dependenceManagerIdList, TypeIdList dependenceComponentList):
    Component(
        Tools::concat(dependenceManagerIdList, { ViewManager::id() }),
        dependenceComponentList)
{

}

void View::init()
{
    manager<ViewManager>()->registerRenderElement(selfPointer<View>());
}

void View::deinit()
{
    manager<ViewManager>()->unregisterRenderElement(selfPointer<View>());
}

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void View::redraw(const mat4 & pMartrix, const mat4 & mvMatrix)
{
    draw(pMartrix, mvMatrix);
}

} // flappy
