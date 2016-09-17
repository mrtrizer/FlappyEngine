#include "resmanager.h"

#include <sstream>

#include "texture.h"

namespace flappy {

using namespace std;
using namespace Tools;

template <>
void ResourceHandler<Atlas>::procNewResource(std::shared_ptr<ResManager> resourceManager)
{
    m_updated = false;
    m_resource = std::move(m_newResource);
    addDependency(resourceManager->get<Texture>(m_resource->dependence()));
}

template<>
unique_ptr<Texture> ResManager::load<Texture>(const string& path) const
{
    return m_resourceLoader->getTexture(path);
}

template<>
unique_ptr<Atlas> ResManager::load<Atlas>(const string& path) const
{
    return m_resourceLoader->getAtlas(path);
}

void ResManager::update(TimeDelta)
{
    for (auto i = m_resourceMap.begin(); i != m_resourceMap.end(); ) {
        i->second->update();
        if (i->second->markedReload())
            i->second->reloadFromSource(shared_from_this());
        if (i->second.use_count() == 1)
            i = m_resourceMap.erase(i);
        else
            i++;
    }
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delim)) {
        elems.push_back(std::move(item));
    }
    return elems;
}

template <>
shared_ptr<ResourceHandler<Quad>> ResManager::get<Quad>(const string& path)
{
    using namespace std;

    if (m_resourceMap.count(path) == 0) {
        auto splittedPath = split(path, ':');
        if (splittedPath.size() != 2)
            throw runtime_error("wrong path format");
        string textureName = splittedPath[1];
        if (splittedPath[0].size() > 0) { // if atlas path, load atlas
            string atlasName = splittedPath[0];// get atlas name
            auto atlas = get<Atlas>(atlasName);
            string quadName = splittedPath[1]; // get quad name in atlas
            auto quadHandler = make_shared<ResourceHandler<Quad>>(path, unique_ptr<Quad>(new Quad(atlas, quadName, shared_from_this())), shared_from_this());
            quadHandler->addDependency(atlas);
            m_resourceMap.emplace(path, quadHandler);
        } else { // if just image path
            string quadName = "__full__";
            string atlasName = string("atlas_full__") + textureName;
            ;{
                auto atlas = Atlas(textureName); // create atlas dependent from image
                atlas.addRect(quadName,{0,0,1,1});
                MGR<ResManager>()->set(atlasName, std::move(atlas));
            }
            auto quadHandler = make_shared<ResourceHandler<Quad>>(path, unique_ptr<Quad>(new Quad(get<Atlas>(atlasName), quadName, shared_from_this())), shared_from_this());
            quadHandler->addDependency(get<Atlas>(atlasName));
            m_resourceMap.emplace(path, quadHandler);
        }
    }
    return dynamic_pointer_cast<ResourceHandler<Quad>>(m_resourceMap[path]);
}

} // flappy
