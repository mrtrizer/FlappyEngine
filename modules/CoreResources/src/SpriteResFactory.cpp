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
    addDependency(ResRepositoryManager::id());
}

std::shared_ptr<ResBase> SpriteResFactory::load(const std::string& name, ExecType execType)  {
    auto splittedName = split(name, ':');

    if (splittedName.size() != 1 && splittedName.size() != 2) {
        LOGE("Can't create quad with name %s. Use format \"atlas:quad\"", name.c_str());
        return nullptr;
    }

    auto textureResManager = manager<ResManager<TextureRes>>();
    auto atlasResManager = manager<ResManager<AtlasRes>>();

    if (splittedName.size() == 2) { // if atlas path, load atlas
        auto repository = manager<ResRepositoryManager>();

        auto meta = repository->findResMeta(splittedName[0]);

        string textureName = meta.data["image"];
        auto texture = textureResManager->getRes(textureName, execType);


        string atlasName = meta.data["meta"];
        auto atlas = atlasResManager->getRes(atlasName, ExecType::SYNC);

        string quadName = splittedName[1];
        auto quad = make_shared<SpriteRes>(atlas, texture, quadName);
        return quad;
    } else { // if just an image path
        string quadName = splittedName[0];
        string defaultQuadName = "__full__";
        auto texture = textureResManager->getRes(quadName, ExecType::SYNC);
        auto atlas = make_shared<AtlasRes>(); // create atlas dependent from image
        atlas->addSpriteInfo(defaultQuadName, AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,1.0f}, texture->size()));
        auto defaultQuad = make_shared<SpriteRes>(atlas, texture, defaultQuadName);
        return defaultQuad;
    }
}

std::shared_ptr<ResBase> SpriteResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
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
