#include "AtlasResFactory.h"

#include <MathUtils.h>
#include <ResManager.h>
#include <JsonRes.h>
#include <AtlasRes.h>

namespace flappy {

AtlasResFactory::AtlasResFactory(Handle<Hierarchy> hierarchy)
    : m_jsonResManager(hierarchy->manager<ResManager<JsonRes>>())
{}

std::shared_ptr<ResBase> AtlasResFactory::load(const std::string& name, ExecType execType) {
    using namespace nlohmann;
    auto jsonRes = m_jsonResManager->getRes(name, execType);
    auto atlasRes = std::make_shared<AtlasRes>(jsonRes);
    auto json = jsonRes->json();
    auto jsonSprites = json["sprites"];
    for (json::iterator jsonIterator = jsonSprites.begin(); jsonIterator != jsonSprites.end(); ++jsonIterator) {
        std::string name = jsonIterator.key();
        float rectX = jsonIterator.value().at("rect_x");
        float rectY = jsonIterator.value().at("rect_y");
        float rectW = jsonIterator.value().at("rect_w");
        float rectH = jsonIterator.value().at("rect_h");
        auto rect = MathUtils::Rect(rectX, rectY, rectX + rectW, rectY + rectH);
        float width = jsonIterator.value().at("width");
        float height = jsonIterator.value().at("height");
        auto size = glm::vec2(width, height);
        atlasRes->addSpriteInfo(name, AtlasRes::SpriteInfo(rect, size));
    }
    return atlasRes;
}

std::shared_ptr<ResBase> AtlasResFactory::create(const std::string& name) {
    auto jsonRes = m_jsonResManager->getRes(name, ExecType::ASYNC);
    return std::make_shared<AtlasRes>(jsonRes);
}

}
