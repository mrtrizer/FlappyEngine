#include "resmanager.h"

#include <sstream>

#include "texture.h"
#include "atlas.h"
#include "quad.h"

namespace flappy {

using namespace std;
using namespace Tools;


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

/// Quad name should contain colon to distinguish image from quad with same name
template <>
void ResManager::initRes<Quad>(std::shared_ptr<ResHandler<Quad>> quadHandler)
{
    auto path = quadHandler->path();
    auto splittedPath = split(path, ':');
    string atlasPath;
    string quadPath;
    if (splittedPath.size() == 2) {
        atlasPath = splittedPath[0];
        quadPath = splittedPath[1];
    } else {
        throw runtime_error("wrong path format");
    }

    if (!atlasPath.empty()) { // if atlas path, load atlas
        auto atlas = get<Atlas>(atlasPath);
        auto quad = unique_ptr<Quad>(new Quad(atlas, quadPath, shared_from_this()));
        quadHandler->setNewResource(std::move(quad), shared_from_this());
        quadHandler->addDependency(atlas);
        m_resourceMap.emplace(path, quadHandler);
    } else { // if just image path
        string textureName = quadPath;
        string defaultQuadName = "__full__";
        string defaultAtlasName = string("atlas_full__") + textureName;
        {
            auto atlas = Atlas(textureName); // create atlas dependent from image
            atlas.addRect(defaultQuadName,{0,0,1,1});
            MGR<ResManager>()->set(defaultAtlasName, std::move(atlas));
        }
        auto defaultQuad = unique_ptr<Quad>(new Quad(get<Atlas>(defaultAtlasName), defaultQuadName, shared_from_this()));
        quadHandler->setNewResource(std::move(defaultQuad), shared_from_this());
        quadHandler->addDependency(get<Atlas>(defaultAtlasName));
    }
}

} // flappy
