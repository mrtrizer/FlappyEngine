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

class ResultValue {
public:
    ResultValue(v8::Isolate* isolate, const v8::UniquePersistent<v8::Context>& globalContext)
        : m_handleScope(std::make_shared<v8::HandleScope>(isolate))
        , m_contextScope(v8::Local <v8::Context>::New (isolate, globalContext))
    {}

    void setValue(v8::Local<v8::Value> value) {
        m_value = value;
    }

    template <typename T>
    T as() {
        return toCpp<T>::cast(m_value);
    }

private:
    std::shared_ptr<v8::HandleScope> m_handleScope;
    v8::Context::Scope m_contextScope;
    v8::Local<v8::Value> m_value;
};

class JSComponent: public Component<JSComponent>
{
public:
    JSComponent(std::string name, std::string script);
    JSComponent(std::string name, std::shared_ptr<TextRes> textRes);

    template <typename ... ArgsT>
    ResultValue call(std::string name, ArgsT ... args);

    ResultValue field(std::string name);

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

};

template <typename ... ArgsT>
ResultValue JSComponent::call(std::string name, ArgsT ... args) {
    ResultValue resultValue(v8::Isolate::GetCurrent(), *manager<V8JSManager>()->context());
    std::vector<v8::Local<v8::Value> > jsArgs;
    wrap(jsArgs, args ...);
    auto jsObject = v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject);
    resultValue.setValue(manager<V8JSManager>()->callMethod(jsObject, name, jsArgs));
    return resultValue;
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
