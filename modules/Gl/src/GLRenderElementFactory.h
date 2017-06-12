#pragma once

#include <RenderElementFactory.h>

#include "GLViewManager.h"

namespace flappy {

class GLRenderElementFactory : public RenderElementFactory {
public:

    GLRenderElementFactory(): RenderElementFactory({ViewManager::id()})
    {}

    std::shared_ptr<Component> createSpriteRender(SafePtr<MeshComponent>) override;
    std::shared_ptr<Component> createMeshRender(SafePtr<MeshComponent>) override;

};

} // flappy
