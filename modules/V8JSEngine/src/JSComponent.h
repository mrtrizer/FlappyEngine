#pragma once

#include <memory>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <v8.h>

#include <Component.h>
#include <V8BasicTypeWrappers.h>

namespace flappy
{

class TextRes;

class JSComponent: public Component<JSComponent>
{
public:
    JSComponent(std::string name, std::string script);
    JSComponent(std::string name, std::shared_ptr<TextRes> textRes);

    template <typename ResultT, typename ... ArgsT>
    ResultT call(std::string name, ArgsT ... args);

    std::string name() const { return m_name; }
    const v8::UniquePersistent<v8::Object>& jsObject() const { return m_jsObject; }

private:
    std::string m_name;
    std::string m_script;
    std::shared_ptr<TextRes> m_textRes;
    v8::UniquePersistent<v8::Object> m_jsObject;

    void init(std::string name, std::string script);

    template <typename ... ArgsT>
    typename std::enable_if<sizeof...(ArgsT) == 0>::type wrap(std::vector<v8::Local<v8::Value> > &) {}

    template <typename ArgT>
    void wrap(std::vector<v8::Local<v8::Value> > &jsArgs, ArgT arg);

    template <typename ArgT, typename ... ArgsT>
    void wrap(std::vector<v8::Local<v8::Value> > &jsArgs, ArgT arg, ArgsT ... args);

    v8::Local<v8::Value> callMethod(std::string name, const std::vector<v8::Local<v8::Value> > &args = {});
};



template <typename ResultT, typename ... ArgsT>
ResultT JSComponent::call(std::string name, ArgsT ... args) {
    v8::HandleScope handleScope(v8::Isolate::GetCurrent());
    v8::Local <v8::Context> context = v8::Local <v8::Context>::New (v8::Isolate::GetCurrent(), *manager<V8JSManager>()->context());
    v8::Context::Scope contextScope (context);
    std::vector<v8::Local<v8::Value> > jsArgs;
    wrap(jsArgs, args ...);
    return toCpp<ResultT>::cast(callMethod(name, jsArgs));
}

template <typename ArgT>
void JSComponent::wrap(std::vector<v8::Local<v8::Value> > &jsArgs, ArgT arg) {
    jsArgs.push_back(toV8<ArgT>::cast(arg));
}

template <typename ArgT, typename ... ArgsT>
void JSComponent::wrap(std::vector<v8::Local<v8::Value> > &jsArgs, ArgT arg, ArgsT ... args) {
    jsArgs.push_back(toV8<ArgT>::cast(arg));
    wrap(jsArgs, args ...);
}

} // flappy
