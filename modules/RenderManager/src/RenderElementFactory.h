#pragma once

#include <memory>

#include <Manager.h>

namespace  flappy {

class MeshComponent;

class RenderElementFactory : public Manager<RenderElementFactory> {
public:
    using Manager::Manager;

    virtual std::shared_ptr<Component> createSpriteRender(SafePtr<MeshComponent>) = 0;
    virtual std::shared_ptr<Component> createMeshRender(SafePtr<MeshComponent>) = 0;
};

} // flappy
