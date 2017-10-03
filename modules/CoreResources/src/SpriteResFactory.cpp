#include "SpriteResFactory.h"

#include <sstream>

#include <Entity.h>

#include "SpriteRes.h"
#include "AtlasRes.h"
#include "TextureRes.h"

namespace flappy {

using namespace std;

SpriteResFactory::SpriteResFactory() {
    addDependency(ResManager<TextureRes>::id());
    addDependency(ResManager<AtlasRes>::id());
}

std::shared_ptr<ResBase> SpriteResFactory::load(const std::string& name)  {
    return create(name);
}

std::shared_ptr<ResBase> SpriteResFactory::create(const std::string& name) {
    auto splittedName = split(name, ':');

    if (splittedName.size() != 1 && splittedName.size() != 3) {
        LOGE("Can't create quad with name %s. Use format \"texture:atlas:quad\"", name.c_str());
        return nullptr;
    }

    auto textureResManager = manager<ResManager<TextureRes>>();
    auto atlasResManager = manager<ResManager<AtlasRes>>();

    if (splittedName.size() == 3) { // if atlas path, load atlas
        string textureName = splittedName[0];
        auto texture = textureResManager->getRes(textureName);

        string atlasName = splittedName[1];
        auto atlas = atlasResManager->getRes(atlasName);

        string quadName = splittedName[2];
        auto quad = make_shared<SpriteRes>(atlas, texture, quadName);
        return quad;
    } else { // if just an image path
        string quadName = splittedName[0];
        string defaultQuadName = "__full__";
        auto texture = textureResManager->getResSync(quadName);
        auto atlas = make_shared<AtlasRes>(); // create atlas dependent from image
        atlas->addSpriteInfo(defaultQuadName, AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,1.0f}, texture->size()));
        auto defaultQuad = make_shared<SpriteRes>(atlas, texture, defaultQuadName);
        return defaultQuad;
    }
}

std::vector<std::string> SpriteResFactory::split(const std::string &s, char delimiter)
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
