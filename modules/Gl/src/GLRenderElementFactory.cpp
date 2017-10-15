#include "GLRenderElementFactory.h"

#include "GLViewSprite.h"
#include "GLViewRect.h"
#include "GLViewText.h"

namespace flappy {

std::shared_ptr<View> GLRenderElementFactory::createSpriteRender(SafePtr<SpriteComponent> spriteComponent) {
    return std::make_shared<GLViewSprite>(spriteComponent);

}

std::shared_ptr<View> GLRenderElementFactory::createMeshRender(SafePtr<MeshComponent> meshComponent) {
    return std::make_shared<GLViewRect>(meshComponent);
}

std::shared_ptr<View> GLRenderElementFactory::createTextRender(SafePtr<TextComponent> textComponent) {
    return std::make_shared<GLViewText>(textComponent);
}

} // flappy
