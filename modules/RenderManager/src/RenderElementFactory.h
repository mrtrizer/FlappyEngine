#pragma once

#include <memory>

#include <Manager.h>
#include <Color.h>

namespace  flappy {

class MeshComponent;
class SpriteComponent;
class TextComponent;
class View;

class RenderElementFactory : public Manager<RenderElementFactory> {
public:
    using Manager::Manager;

    virtual std::shared_ptr<View> createSpriteRender(SafePtr<SpriteComponent>) = 0;
    virtual std::shared_ptr<View> createMeshRender(SafePtr<MeshComponent>) = 0;
    virtual std::shared_ptr<View> createTextRender(SafePtr<TextComponent>) = 0;

};

} // flappy
