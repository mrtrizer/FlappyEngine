#include "MaterialResFactory.h"

#include <stdexcept>
#include <regex>

#include <MaterialRes.h>
#include <ResManager.h>
#include <JsonRes.h>
#include <TextureRes.h>
#include <ShaderRes.h>

namespace flappy {

MaterialResFactory::MaterialResFactory()
{
    this->addDependency(ResManager<JsonRes>::id());
    this->addDependency(ResManager<TextureRes>::id());
    this->addDependency(ResManager<ShaderRes>::id());
}

std::shared_ptr<ResBase> MaterialResFactory::load(const std::string& name, ExecType execType) {
    auto jsonRes = manager<ResManager<JsonRes>>()->getRes(name, ExecType::SYNC);

    const auto& jsonObject = jsonRes->json();
    std::string shaderName = jsonObject["shader"];
    auto shaderRes = manager<ResManager<ShaderRes>>()->getRes(shaderName, ExecType::SYNC);
    auto materialRes = std::make_shared<MaterialRes>(jsonRes, shaderRes);
    try {
        std::string renderMode = jsonObject.at("render_mode");
        materialRes->setRenderModeFromStr(renderMode);
    } catch (const std::exception&) {
        LOGI("Default render mode is set: TRIANGLES");
    }
    auto uniformsObject = jsonObject["uniforms"];
    for (auto iter = uniformsObject.begin(); iter != uniformsObject.end(); iter++) {
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
        if (type == "vec3") {
            materialRes->setVec3(name, glm::vec3(value[0], value[1], value[2]));
        }
        if (type == "vec2") {
            materialRes->setVec2(name, glm::vec2(value[0], value[1]));
        }
        if (type == "float") {
            materialRes->setFloat(name, value);
        }
        if (type == "int") {
            materialRes->setInt(name, value);
        }
    }
    return materialRes;
}

std::shared_ptr<ResBase> MaterialResFactory::create(const std::string& name) {
    return load(name, ExecType::ASYNC);
}

} // flappy
