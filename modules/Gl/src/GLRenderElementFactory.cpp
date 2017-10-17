#include "GLRenderElementFactory.h"

#include "GLSpriteRender.h"
#include "GLMeshRender.h"
#include "GLTextRender.h"

namespace flappy {

std::shared_ptr<Render> GLRenderElementFactory::createSpriteRender(SafePtr<SpriteComponent> spriteComponent) {
    return std::make_shared<GLSpriteRender>(spriteComponent);

}

std::shared_ptr<Render> GLRenderElementFactory::createMeshRender(SafePtr<MeshComponent> meshComponent) {
    return std::make_shared<GLMeshRender>(meshComponent);
}

std::shared_ptr<Render> GLRenderElementFactory::createTextRender(SafePtr<TextComponent> textComponent) {
    return std::make_shared<GLTextRender>(textComponent);
}

} // flappy
