#pragma once

#include <managers/ResManager/Res.h>
#include "TextRes.h"
#include "json.hpp"

namespace flappy {

class JsonRes: public Res
{
public:
    JsonRes(std::shared_ptr<TextRes> textRes):
        m_textRes(textRes)
    {
        setFromStr(textRes->text());
    }

    std::list<std::shared_ptr<Res>> dependencyList() const override {
        return {m_textRes};
    }

    void setFromStr(const std::string& jsonStr) {
        if (!jsonStr.empty())
            m_json = nlohmann::json::parse(jsonStr);
        else
            m_json.clear();
    }

    std::string toStr() const {
        return m_json.dump();
    }

    nlohmann::json& json() {
        return m_json;
    }

    template <typename JsonT>
    void setJson(JsonT& json) {
        json = json;
    }

private:
    std::shared_ptr<TextRes> m_textRes;
    nlohmann::json m_json;
};

} // flappy
