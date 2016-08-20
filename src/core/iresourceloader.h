#pragma once

#include <memory>

namespace flappy {

class Texture;
class Atlas;

class IResourceLoader
{
public:
    virtual ~IResourceLoader(){}
    virtual std::unique_ptr<Texture> getTexture(const std::string& name) const = 0;
    virtual std::unique_ptr<Atlas> getAtlas(const std::string& name) const = 0;
};

} // flappy
