#include "JsonRes.h"

namespace flappy {

JsonRes::JsonRes(std::shared_ptr<TextRes> textRes):
    m_textRes(textRes)
{
    setFromStr(textRes->text());
}

std::list<std::shared_ptr<Res>> JsonRes::dependencyList() const {
    return {m_textRes};
}

void JsonRes::setFromStr(const std::string& jsonStr) {
    if (!jsonStr.empty())
        m_json = nlohmann::json::parse(jsonStr);
    else
        m_json.clear();
}

std::string JsonRes::toStr() const {
    return m_json.dump();
}

nlohmann::json& JsonRes::json() {
    return m_json;
}

} // flappy

