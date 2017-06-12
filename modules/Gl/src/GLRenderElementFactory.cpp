#include "GLRenderElementFactory.h"

#include "GLViewSprite.h"
#include "GLViewRect.h"
#include "GLViewCircle.h"

namespace flappy {

std::shared_ptr<Component> createSpriteRender(SafePtr<MeshComponent>) {
    return std::make_shared<GLViewRect>();
}

std::shared_ptr<Component> createMeshRender(SafePtr<MeshComponent>) {
    return std::make_shared<GLViewRect>();
}

} // flappy
