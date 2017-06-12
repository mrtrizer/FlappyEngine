#include "GLRenderElementFactory.h"

#include "GLViewSprite.h"
#include "GLViewRect.h"
#include "GLViewCircle.h"

namespace flappy {

std::shared_ptr<View> GLRenderElementFactory::createSpriteRender(SafePtr<MeshComponent>) {
    return std::make_shared<GLViewRect>();
}

std::shared_ptr<View> GLRenderElementFactory::createMeshRender(SafePtr<MeshComponent>) {
    return std::make_shared<GLViewRect>();
}

} // flappy
