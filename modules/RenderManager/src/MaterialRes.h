#pragma once

#include <unordered_map>
#include <string>

#include <json/json.hpp>
#include <glm/glm.hpp>

#include <TextureRes.h>
#include <JsonRes.h>
#include <Res.h>

namespace flappy {

class MaterialRes: public Res<MaterialRes>
{
public:
    MaterialRes(std::shared_ptr<JsonRes> jsonRes)
        : m_jsonRes(jsonRes)
    {}

    void setVec4(std::string name, glm::vec4 vector) { m_vec4Map.emplace(name, vector); }
    glm::vec4 getVec4(std::string name) { return m_vec4Map[name]; }

    void setTextureRes(std::string name, std::shared_ptr<TextureRes> textureRes) { m_textureResMap[name] = textureRes; }
    std::shared_ptr<TextureRes> getTextureRes(std::string name) { return m_textureResMap[name]; }

    std::unordered_map<std::string, glm::vec4> vec4Map() { return m_vec4Map; }
    std::unordered_map<std::string, std::shared_ptr<TextureRes>> textureResMap() { return m_textureResMap; }

    std::list<std::shared_ptr<ResBase>> dependencyList() const final { return {m_jsonRes}; }

private:
    std::unordered_map<std::string, glm::vec4> m_vec4Map;
    std::unordered_map<std::string, std::shared_ptr<TextureRes>> m_textureResMap;
    std::shared_ptr<JsonRes> m_jsonRes;
};

} // flappy
