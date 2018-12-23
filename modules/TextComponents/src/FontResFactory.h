#pragma once

#include <memory>
#include <vector>

#include <ResFactory.h>

#include "FontRes.h"

namespace flappy {

class FontResFactory: public ResFactory<FontRes> {
public:
    FontResFactory(Handle<Hierarchy> hierarchy);

    std::shared_ptr<ResBase> load(const std::string& name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string& name) final;
private:
    Handle<ResManager<TextureRes>> m_textureResManager;
    Handle<ResManager<GlyphSheetRes>> m_glyphSheetResManager;
    Handle<ResRepositoryManager> m_resRepositoryManager;
};

} // flappy
