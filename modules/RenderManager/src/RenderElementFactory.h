#pragma once

#include <memory>

#include <Color.h>
#include <Handle.hpp>

namespace  flappy {

class Entity;

class RenderElementFactory {
public:
    virtual ~RenderElementFactory() = default;
    virtual AnyHandle createSpriteRender(Handle<Entity>) = 0;
    virtual AnyHandle createMeshRender(Handle<Entity>) = 0;
    virtual AnyHandle createTextRender(Handle<Entity>) = 0;

};

} // flappy
