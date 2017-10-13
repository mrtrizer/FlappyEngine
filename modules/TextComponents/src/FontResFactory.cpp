#include "FontResFactory.h"

#include <sstream>

#include <Entity.h>
#include <ResRepositoryManager.h>

#include "FontRes.h"
#include "GlyphSheetRes.h"
#include "TextureRes.h"

namespace flappy {

using namespace std;

FontResFactory::FontResFactory() {
    addDependency(ResManager<TextureRes>::id());
    addDependency(ResManager<GlyphSheetRes>::id());
    addDependency(ResRepositoryManager::id());
}

std::shared_ptr<ResBase> FontResFactory::load(const std::string& name, ExecType execType)  {
    auto repository = manager<ResRepositoryManager>();
    auto meta = repository->findResMeta(name);

    auto textureResManager = manager<ResManager<TextureRes>>();
    std::string textureName = meta.data["image"];
    auto texture = textureResManager->getRes(textureName, execType);

    auto atlasResManager = manager<ResManager<GlyphSheetRes>>();
    std::string glyphSheetName = meta.data["meta"];
    auto glyphSheet = atlasResManager->getRes(glyphSheetName, ExecType::SYNC);

    return std::make_shared<FontRes>(glyphSheet, texture);
}

std::shared_ptr<ResBase> FontResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
