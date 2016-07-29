#pragma once

#include <core/manager.h>
#include <core/texture.h>

class ResourceMgr: public Manager<ResourceMgr>
{
public:
    virtual std::shared_ptr<Texture> getTexture(const std::string& path) const = 0 ;

    template <typename Type>
    std::shared_ptr<Type> get(const std::string& path) const {}
};
