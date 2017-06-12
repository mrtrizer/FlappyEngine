#pragma once

#include <memory>

#include <Manager.h>

namespace  flappy {

class MeshComponent;
class View;

class RenderElementFactory : public Manager<RenderElementFactory> {
public:
    using Manager::Manager;

    virtual std::shared_ptr<View> createSpriteRender(SafePtr<MeshComponent>) = 0;
    virtual std::shared_ptr<View> createMeshRender(SafePtr<MeshComponent>) = 0;
};

} // flappy
