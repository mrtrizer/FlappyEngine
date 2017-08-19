#include "QuadResFactory.h"

#include <sstream>

#include <ResManager.h>

#include "QuadRes.h"
#include "AtlasRes.h"
#include "TextureRes.h"

namespace flappy {

using namespace std;

std::shared_ptr<Res> QuadResFactory::load(const std::string& name, SafePtr<Entity> resManager)  {
    auto splittedName = split(name, ':');
    if (splittedName.size() == 2) { // if atlas path, load atlas
        string atlasName = splittedName[0];
        string quadName = splittedName[1];
        auto atlas = resManager->getResSync<AtlasRes>(atlasName);
        auto texture = resManager->getRes<TextureRes>(atlasName);
        auto quad = make_shared<QuadRes>(atlas, texture, quadName);
        return quad;
    } else { // if just an image path
        string quadName = splittedName[0];
        string textureName = quadName;
        string defaultQuadName = "__full__";
        string defaultAtlasName = string("atlas_full__") + textureName;
        auto texture = resManager->getResSync<TextureRes>(quadName);
        {
            auto atlas = make_shared<AtlasRes>(); // create atlas dependent from image
            atlas->addSpriteInfo(defaultQuadName, AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,1.0f}, texture->size()));
            resManager->setRes<AtlasRes>(defaultAtlasName, atlas);
        }
        auto defaultAtlas = resManager->getRes<AtlasRes>(defaultAtlasName);
        auto defaultQuad = make_shared<QuadRes>(defaultAtlas, texture, defaultQuadName);
        return defaultQuad;
    }
}

std::vector<std::string> QuadResFactory::split(const std::string &s, char delimiter)
{
    std::stringstream ss(s);
    std::string item;
    std::vector<std::string> elems;
    while (std::getline(ss, item, delimiter)) {
        elems.push_back(std::move(item));
    }
    return elems;
}

} // flappy
