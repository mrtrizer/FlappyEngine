#include "GLRenderElementFactory.h"

#include "GLSpriteRender.h"
#include "GLMeshRender.h"
#include "GLTextRender.h"

namespace flappy {

GLRenderElementFactory::GLRenderElementFactory(Handle<Hierarchy> hierarchy)
{
    USER_ASSERT(hierarchy->manager<RenderManager>().isValid());
}
    
AnyHandle GLRenderElementFactory::createSpriteRender(Handle<Entity> entity) {
    return entity->createComponent<GLSpriteRender>();
}

AnyHandle GLRenderElementFactory::createMeshRender(Handle<Entity> entity) {
    return entity->createComponent<GLMeshRender>();
}

AnyHandle GLRenderElementFactory::createTextRender(Handle<Entity> entity) {
    return entity->createComponent<GLTextRender>();
}

} // flappy
