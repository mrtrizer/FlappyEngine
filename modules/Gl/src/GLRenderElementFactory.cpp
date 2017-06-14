#include "GLRenderElementFactory.h"

#include "GLViewSprite.h"
#include "GLViewRect.h"

namespace flappy {

std::shared_ptr<View> GLRenderElementFactory::createSpriteRender(SafePtr<SpriteComponent> spriteComponent) {
    return std::make_shared<GLViewSprite>(spriteComponent);

}

std::shared_ptr<View> GLRenderElementFactory::createMeshRender(SafePtr<MeshComponent>) {
    return std::make_shared<GLViewRect>();
}

} // flappy
