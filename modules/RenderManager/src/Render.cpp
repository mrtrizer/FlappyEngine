#include "Render.h"

#include <RenderManager.h>
#include <Tools.h>

namespace flappy {

using namespace glm;
using namespace std;

Render::Render()
{
    addDependency(RenderManager::id());

    subscribe([this](InitEvent) {
        manager<RenderManager>()->registerRenderElement(selfPointer());
    });

    subscribe([this](DeinitEvent) {
        manager<RenderManager>()->unregisterRenderElement(selfPointer());
    });
}

/// Calls update method if need (if externUpdate() was invoked before)
/// and than calls virtual draw() method, reimplemented in concrete childs.
void Render::redraw(const mat4 & pMartrix, const mat4 & mvMatrix)
{
    draw(pMartrix, mvMatrix);
}

} // flappy
