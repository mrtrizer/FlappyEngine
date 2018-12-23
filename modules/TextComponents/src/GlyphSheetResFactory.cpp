#include "GlyphSheetResFactory.h"

#include <ResManager.h>
#include <JsonRes.h>
#include "GlyphSheetRes.h"

namespace flappy {

GlyphSheetResFactory::GlyphSheetResFactory(Handle<Hierarchy> hierarchy)
    : m_jsonResManager(hierarchy->manager<ResManager<JsonRes>>())
{}

std::shared_ptr<ResBase> GlyphSheetResFactory::load(const std::string& name, ExecType execType) {
    using namespace nlohmann;
    auto jsonRes = m_jsonResManager->getRes(name, execType);
    auto glyphSheetRes = std::make_shared<GlyphSheetRes>(jsonRes);
    auto jsonData = jsonRes->json();

    auto jsonInfo = jsonData["info"];
    GlyphSheetRes::Info info;
    info.face = jsonInfo.at("face");
    info.size = jsonInfo.at("size");
    info.bold = jsonInfo.at("bold");
    info.italic = jsonInfo.at("italic");
//    info.charset = jsonInfo.at("charset");
    info.unicode = jsonInfo.at("unicode");
    info.stretchH = jsonInfo.at("stretchH");
    info.smooth = jsonInfo.at("smooth");
    info.aa = jsonInfo.at("aa");
//    info.padding = jsonInfo.at("padding");
//    info.spacing = jsonInfo.at("spacing");
    glyphSheetRes->setInfo(info);

    auto jsonCommon = jsonData["common"];
    GlyphSheetRes::Common common;
    common.lineHeight = jsonCommon.at("lineHeight");
    common.base = jsonCommon.at("base");
    common.scaleW = jsonCommon.at("scaleW");
    common.scaleH = jsonCommon.at("scaleH");
    common.pages = jsonCommon.at("pages");
    common.packed = jsonCommon.at("packed");
    common.alphaChnl = jsonCommon.at("alphaChnl");
    common.redChnl = jsonCommon.at("redChnl");
    common.greenChnl = jsonCommon.at("greenChnl");
    common.blueChnl = jsonCommon.at("blueChnl");
    glyphSheetRes->setCommon(common);

    auto jsonChars = jsonData["chars"];
    for (json::iterator jsonIterator = jsonChars.begin(); jsonIterator != jsonChars.end(); ++jsonIterator) {
        json value = jsonIterator.value();
        GlyphSheetRes::Glyph glyph;
        glyph.id = value.at("id");
        glyph.x = value.at("x");
        glyph.y = value.at("y");
        glyph.width = value.at("width");
        glyph.height = value.at("height");
        glyph.xoffset = value.at("xoffset");
        glyph.yoffset = value.at("yoffset");
        glyph.xadvance = value.at("xadvance");
        glyph.page = value.at("page");
        glyph.chnl = value.at("chnl");
        glyphSheetRes->addGlyph((char32_t)glyph.id, glyph);
    }

    auto jsonKernings = jsonData["kernings"];
    for (json::iterator jsonIterator = jsonKernings.begin(); jsonIterator != jsonKernings.end(); ++jsonIterator) {
        json value = jsonIterator.value();
        GlyphSheetRes::Kerning kerning;
        kerning.first = value.at("first");
        kerning.second = value.at("second");
        kerning.amount = value.at("amount");
        glyphSheetRes->addKerning(kerning);
    }
    return glyphSheetRes;
}

std::shared_ptr<ResBase> GlyphSheetResFactory::create(const std::string& name) {
    auto jsonRes = m_jsonResManager->getRes(name, ExecType::ASYNC);
    return std::make_shared<GlyphSheetRes>(jsonRes);
}

}
