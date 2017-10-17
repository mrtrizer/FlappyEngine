#pragma once

#include <RenderElementFactory.h>

#include "GLRenderManager.h"

namespace flappy {

class GLRenderElementFactory : public RenderElementFactory {
public:

    GLRenderElementFactory()
    {
        addDependency(RenderManager::id());
    }

    std::shared_ptr<Render> createSpriteRender(SafePtr<SpriteComponent>) final;
    std::shared_ptr<Render> createMeshRender(SafePtr<MeshComponent>) final;
    std::shared_ptr<Render> createTextRender(SafePtr<TextComponent>) final;

};

} // flappy
