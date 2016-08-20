#pragma once

#include <string>

#include <core/iresourceloader.h>
#include <core/atlas.h>
#include <core/texture.h>

namespace flappy {

class SDLResourceLoader :public IResourceLoader
{
public:
    SDLResourceLoader(std::string path);
    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const override;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const override {return nullptr;}

    class file_open_error {};

class wrong_bitmap_format {};
private:
    std::string m_path;
};

} // flappy
