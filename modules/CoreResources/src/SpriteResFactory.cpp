#include "SpriteResFactory.h"

#include "SpriteRes.h"
#include "AtlasRes.h"
#include "TextureRes.h"

#include <Utility.hpp>
#include <StringUtils.hpp>
#include <DebugServices.h>

namespace flappy {

using namespace std;

SpriteResFactory::SpriteResFactory(Handle<Hierarchy> hierarchy)
    : m_textureResManager(hierarchy->manager<ResManager<TextureRes>>())
    , m_atlasResManager(hierarchy->manager<ResManager<AtlasRes>>())
    , m_resRepositoryManager(hierarchy->manager<ResRepositoryManager>())
{}

std::shared_ptr<ResBase> SpriteResFactory::load(const std::string& name, ExecType execType)  {
    auto splittedName = StringUtils::split(name, ':');

    if (splittedName.size() != 1 && splittedName.size() != 2) {
        LOGE("Can't create quad with name %s. Use format \"atlas:quad\"", name.c_str());
        return nullptr;
    }


    if (splittedName.size() == 2) { // if atlas path, load atlas
        auto meta = m_resRepositoryManager->findResMeta(splittedName[0]);

        string textureName = meta.data["image"];
        auto texture = m_textureResManager->getRes(textureName, execType);


        string atlasName = meta.data["meta"];
        auto atlas = m_atlasResManager->getRes(atlasName, ExecType::SYNC);

        string quadName = splittedName[1];
        auto quad = make_shared<SpriteRes>(atlas, texture, quadName);
        return quad;
    } else { // if just an image path
        string quadName = splittedName[0];
        string defaultQuadName = "__full__";
        auto texture = m_textureResManager->getRes(quadName, ExecType::SYNC);
        auto atlas = make_shared<AtlasRes>(); // create atlas dependent from image
        atlas->addSpriteInfo(defaultQuadName, AtlasRes::SpriteInfo({0.0f,0.0f,1.0f,1.0f}, texture->size()));
        auto defaultQuad = make_shared<SpriteRes>(atlas, texture, defaultQuadName);
        return defaultQuad;
    }
}

std::shared_ptr<ResBase> SpriteResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
