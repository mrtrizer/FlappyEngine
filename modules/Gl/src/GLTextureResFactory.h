#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>
#include <GLTextureRes.h>
#include <Handle.hpp>

namespace flappy {
    
class Hierarchy;
class IRgbaBitmapRes;
template <typename T>
class ResManager;

class GLTextureResFactory: public ResFactory<TextureRes> {
public:
    GLTextureResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
private:
    Handle<ResManager<IRgbaBitmapRes>> m_bitmapResManager;
    Handle<Entity> m_rootEntity;
};

} // flappy
