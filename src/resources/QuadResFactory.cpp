#include "QuadResFactory.h"

#include <sstream>

#include "QuadRes.h"
#include "AtlasRes.h"
#include "TextureRes.h"
#include <managers/ResManager/ResManager.h>

namespace flappy {

using namespace std;

std::shared_ptr<Res> QuadResFactory::load(const std::string& name, std::shared_ptr<ResManager> resManager)  {
    auto splittedName = split(name, ':');
    string atlasName;
    string quadName;
    if (splittedName.size() == 2) {
        atlasName = splittedName[0];
        quadName = splittedName[1];
    } else {
        throw runtime_error("Wrong quad name format.");
    }
    if (!atlasName.empty()) { // if atlas path, load atlas
        auto atlas = resManager->getRes<AtlasRes>(atlasName);
        auto texture = resManager->getRes<TextureRes>(atlasName);
        auto quad = make_shared<QuadRes>(atlas, texture, quadName);
        return quad;
    } else { // if just an image path
        string textureName = quadName;
        string defaultQuadName = "__full__";
        string defaultAtlasName = string("atlas_full__") + textureName;
        {
            auto atlas = make_shared<AtlasRes>(); // create atlas dependent from image
            atlas->addRect(defaultQuadName, {0,0,1,1});
            resManager->setRes<AtlasRes>(defaultAtlasName, atlas);
        }
        auto defaultAtlas = resManager->getRes<AtlasRes>(defaultAtlasName);
        auto texture = resManager->getRes<TextureRes>(quadName);
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
