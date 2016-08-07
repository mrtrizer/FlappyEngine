#pragma once

#include <map>

#include <core/manager.h>

namespace flappy {

using namespace std;

class Texture;

//class Resource {
//public:
//    virtual ~Resource() noexcept {}
//};

class ResourceMgr: public Manager<ResourceMgr>
{
public:
    virtual shared_ptr<Texture> getTexture(const string& path) const = 0 ;

    template <typename Type>
    shared_ptr<Type> get(const string& path) const {}
};

//class ResourcePack {
//public:
//    template <typename ResType>
//    shared_ptr<ResType> getRes(const string& name) {
//        return m_resourceMap[name];
//    }

//    template <typename ResType>
//    void addRes(const string& name) {
//        m_resourceMap[name] = m_resourceMgr.get<ResType>(name);
//    }

//private:
//    map<string, shared_ptr<Resource>> m_resourceMap;
//    shared_ptr<ResourceMgr> m_resourceMgr;
//};

//void test(){
//auto pack = Resource::createPack("main_pack");
//pack.addResource<Texture>("test");
//pack.addResource<Texture>("test2");
//pack.load();

//}

} // flappy
