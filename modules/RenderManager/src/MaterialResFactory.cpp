#include "MaterialResFactory.h"

#include <regex>

#include <MaterialRes.h>
#include <ResManager.h>
#include <JsonRes.h>
#include <TextureRes.h>

namespace flappy {

MaterialResFactory::MaterialResFactory()
{
    this->addDependency(ResManager<JsonRes>::id());
    this->addDependency(ResManager<TextureRes>::id());
}

std::shared_ptr<ResBase> MaterialResFactory::load(const std::string& name, ExecType execType) {
    auto jsonRes = manager<ResManager<JsonRes>>()->getRes(name, execType);
    auto materialRes = std::make_shared<MaterialRes>(jsonRes);
    const auto& jsonObject = jsonRes->json();
    for (auto iter = jsonObject.begin(); iter != jsonObject.end(); iter++) {
        std::regex regex("\\((.*?)\\)\\s*(.*)");
        std::smatch match;
        std::string key = iter.key();
        std::regex_search(key, match, regex);
        auto type = match[1].str();
        auto name = match[2].str();
        auto value = iter.value();
        LOG("%s : %s", type.c_str(), name.c_str());
        if (type == "texture") {
            std::string textureName = value;
            auto texture = manager<ResManager<TextureRes>>()->getRes(textureName, execType);
            materialRes->setTextureRes(name, texture);
        }

        if (type == "vec4") {
            materialRes->setVec4(name, glm::vec4(value[0], value[1], value[2], value[3]));
        }
    }
    return materialRes;
}

std::shared_ptr<ResBase> MaterialResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
