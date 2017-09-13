#pragma once

#include <RenderElementFactory.h>

#include "GLViewManager.h"

namespace flappy {

class GLRenderElementFactory : public RenderElementFactory {
public:

    GLRenderElementFactory()
    {
        addDependency(ViewManager::id());
    }

    std::shared_ptr<View> createSpriteRender(SafePtr<SpriteComponent>) final;
    std::shared_ptr<View> createMeshRender(SafePtr<MeshComponent>) final;

};

} // flappy
