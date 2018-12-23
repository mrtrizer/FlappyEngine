#include "FontResFactory.h"

#include <sstream>

#include <ResRepositoryManager.h>

#include "FontRes.h"
#include "GlyphSheetRes.h"
#include "TextureRes.h"

namespace flappy {

using namespace std;

FontResFactory::FontResFactory(Handle<Hierarchy> hierarchy)
    : m_textureResManager(hierarchy->manager<ResManager<TextureRes>>())
    , m_glyphSheetResManager(hierarchy->manager<ResManager<GlyphSheetRes>>())
    , m_resRepositoryManager(hierarchy->manager<ResRepositoryManager>())
{}

std::shared_ptr<ResBase> FontResFactory::load(const std::string& name, ExecType execType)  {
    auto meta = m_resRepositoryManager->findResMeta(name);

    std::string textureName = meta.data["image"];
    auto texture = m_textureResManager->getRes(textureName, execType);

    auto atlasResManager = m_glyphSheetResManager;
    std::string glyphSheetName = meta.data["meta"];
    auto glyphSheet = atlasResManager->getRes(glyphSheetName, ExecType::SYNC);

    return std::make_shared<FontRes>(glyphSheet, texture);
}

std::shared_ptr<ResBase> FontResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
