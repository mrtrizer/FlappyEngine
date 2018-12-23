#pragma once

#include <RenderElementFactory.h>

#include "GLRenderManager.h"

namespace flappy {

class GLRenderElementFactory : public RenderElementFactory {
public:
    GLRenderElementFactory(Handle<Hierarchy> hierarchy);
    
    AnyHandle createSpriteRender(Handle<Entity>) final;
    AnyHandle createMeshRender(Handle<Entity>) final;
    AnyHandle createTextRender(Handle<Entity>) final;

};

} // flappy
