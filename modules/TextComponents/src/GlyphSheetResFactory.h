#pragma once

#include <string>

#include <ResFactory.h>
#include "GlyphSheetRes.h"

namespace flappy {

class GlyphSheetResFactory :public ResFactory<GlyphSheetRes>
{
public:
    GlyphSheetResFactory();
    std::shared_ptr<ResBase> load(const std::string&name, ExecType execType) final;
    std::shared_ptr<ResBase> create(const std::string&) final;
};

} // flappy
