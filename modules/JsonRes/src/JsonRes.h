#pragma once

#include <ResManager.h>
#include <TextRes.h>
#include <json/json.hpp>

namespace flappy {

class JsonRes: public Res<JsonRes>
{
public:
    JsonRes() = default;
    JsonRes(std::shared_ptr<TextRes> textRes);

    std::list<std::shared_ptr<ResBase>> dependencyList() const final;
    void setFromStr(const std::string& jsonStr);
    std::string toStr() const;
    nlohmann::json& json();

    template <typename JsonT>
    void setJson(JsonT& json) {
        json = json;
    }

private:
    std::shared_ptr<TextRes> m_textRes;
    nlohmann::json m_json;
};

} // flappy
