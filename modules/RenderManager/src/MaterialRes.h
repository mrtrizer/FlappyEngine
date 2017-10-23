#pragma once

#include <unordered_map>
#include <string>

#include <json/json.hpp>
#include <glm/glm.hpp>

#include <TextureRes.h>
#include <JsonRes.h>
#include <ShaderRes.h>
#include <Res.h>

namespace flappy {

class MaterialRes: public Res<MaterialRes>
{
public:
    enum class RenderMode {
        POINTS,
        LINES,
        TRIANGLES,
        TRIANGLE_STRIP,
        TRIANGLE_FAN,
    };

    MaterialRes(std::shared_ptr<JsonRes> jsonRes, std::shared_ptr<ShaderRes> shaderRes)
        : m_jsonRes(jsonRes)
        , m_shaderRes(shaderRes)
    {}

    void setRenderMode(RenderMode renderMode) { m_renderMode = renderMode; }
    void setRenderModeFromStr(std::string mode);
    RenderMode renderMode() { return m_renderMode; }

    void setVec4(std::string name, glm::vec4 vector) { m_vec4Map.emplace(name, vector); }

    void setVec3(std::string name, glm::vec3 vector) { m_vec3Map.emplace(name, vector); }

    void setVec2(std::string name, glm::vec2 vector) { m_vec2Map.emplace(name, vector); }

    void setFloat(std::string name, float value) { m_floatMap.emplace(name, value); }

    void setInt(std::string name, int value) { m_intMap.emplace(name, value); }

    void setTextureRes(std::string name, std::shared_ptr<TextureRes> textureRes) { m_textureResMap[name] = textureRes; }

    const std::unordered_map<std::string, glm::vec4>& vec4Map() const { return m_vec4Map; }
    const std::unordered_map<std::string, glm::vec3>& vec3Map() const { return m_vec3Map; }
    const std::unordered_map<std::string, glm::vec2>& vec2Map() const { return m_vec2Map; }
    const std::unordered_map<std::string, float>& floatMap() const { return m_floatMap; }
    const std::unordered_map<std::string, int>& intMap() const { return m_intMap; }
    const std::unordered_map<std::string, std::shared_ptr<TextureRes>>& textureResMap() const { return m_textureResMap; }
    std::shared_ptr<ShaderRes> shaderRes() { return m_shaderRes; }

    std::list<std::shared_ptr<ResBase>> dependencyList() const final { return {m_jsonRes}; }

private:
    std::unordered_map<std::string, RenderMode> m_renderModes {
        {"points", RenderMode::POINTS},
        {"lines", RenderMode::LINES},
        {"triangles", RenderMode::TRIANGLES},
        {"triangle_strip", RenderMode::TRIANGLE_STRIP},
        {"triangle_fan", RenderMode::TRIANGLE_FAN}
    };
    RenderMode m_renderMode = RenderMode::TRIANGLES;
    std::unordered_map<std::string, glm::vec4> m_vec4Map;
    std::unordered_map<std::string, glm::vec3> m_vec3Map;
    std::unordered_map<std::string, glm::vec2> m_vec2Map;
    std::unordered_map<std::string, float> m_floatMap;
    std::unordered_map<std::string, int> m_intMap;
    std::unordered_map<std::string, std::shared_ptr<TextureRes>> m_textureResMap;
    std::shared_ptr<JsonRes> m_jsonRes;
    std::shared_ptr<ShaderRes> m_shaderRes;
};

} // flappy
