#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <v8.h>

#include <Component.h>

namespace flappy
{

class TextRes;

class JSComponent: public Component<JSComponent>
{
public:
    JSComponent(std::string name, std::string script);
    JSComponent(std::string name, std::shared_ptr<TextRes> textRes);

    void callMethod(std::string name, const std::vector<v8::Local<v8::Value> > &args = {});
    void init(std::string name, std::string script);

    std::string name() const { return m_name; }
    const v8::UniquePersistent<v8::Object>& jsObject() const { return m_jsObject; }

private:
    std::string m_name;
    std::string m_script;
    std::shared_ptr<TextRes> m_textRes;
    v8::UniquePersistent<v8::Object> m_jsObject;
};

} // flappy
