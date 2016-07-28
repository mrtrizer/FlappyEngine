#ifndef RESOURCEMGR_H
#define RESOURCEMGR_H

#include <core/manager.h>
#include <core/texture.h>

class ResourceMgr: public Manager<ResourceMgr>
{
public:
    virtual std::shared_ptr<Texture> getTexture(std::string name) const = 0 ;

    template <typename Type>
    std::shared_ptr<Type> get(std::string path) const {}
};

#endif // RESOURCEMGR_H
