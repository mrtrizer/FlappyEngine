#pragma once

#include <memory>
#include <vector>

#include <IResFactory.h>
#include <GLTextureRes.h>

namespace flappy {

class GLTextureResFactory: public ResFactory<TextureRes> {
public:
    GLTextureResFactory();

    std::shared_ptr<ResBase> load(const std::string& name) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
};

} // flappy
