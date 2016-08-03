#pragma once

#include <core/manager.h>
#include <core/texture.h>

namespace flappy {

using namespace std;

class ResourceMgr: public Manager<ResourceMgr>
{
public:
    virtual shared_ptr<Texture> getTexture(const string& path) const = 0 ;

    template <typename Type>
    shared_ptr<Type> get(const string& path) const {}
};

} // flappy
