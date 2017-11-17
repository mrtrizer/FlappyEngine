#include "JSComponent.h"

#include <v8.h>

#include <Entity.h>
#include <TextRes.h>

#include "V8JSManager.h"

namespace flappy {

using namespace v8;

JSComponent::JSComponent(std::string name, std::string script)
    : m_name(name)
    , m_script(script)
{
    addDependency(V8JSManager::id());

    subscribe([this, name, script](InitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        init(name, script);
    });

    subscribe([this, name, script](DeinitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        callMethod( "deinit");
    });

    subscribe([this, name, script](UpdateEvent e) {
        HandleScope handleScope(Isolate::GetCurrent());

        auto jsDt = v8::Number::New(Isolate::GetCurrent(), e.dt);

       callMethod("update", {jsDt});
    });
}

JSComponent::JSComponent(std::string name, std::shared_ptr<TextRes> textRes)
    : m_name(name)
    , m_textRes(textRes)
{
    addDependency(V8JSManager::id());

    subscribe([this, name, textRes](InitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        init(name, textRes->text());
    });

    subscribe([this, name, textRes](DeinitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        callMethod( "deinit");
        m_jsObject.Reset();
    });

    subscribe([this, name, textRes](UpdateEvent e) {
        if (isInitialized()) {
            HandleScope handleScope(Isolate::GetCurrent());

            // reinitialization of a script
            if (m_textRes->lastRes() != m_textRes) {
                m_textRes = m_textRes->lastRes();
                callMethod( "deinit");
                init(name, textRes->text());
            }

            auto jsDt = v8::Number::New(Isolate::GetCurrent(), e.dt);

            callMethod("update", {jsDt});
        }
    });
}

Local<Value> JSComponent::callMethod(std::string name, const std::vector<v8::Local<v8::Value>>& args) {
    auto jsObject = v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject);
    return manager<V8JSManager>()->callMethod(jsObject, name, args);
}

void JSComponent::init(std::string name, std::string script) {
    m_jsObject = manager<V8JSManager>()->runJSComponent(name, script, selfPointer());
    if (!m_jsObject.IsEmpty())
        callMethod("init");
    else
        throw std::runtime_error("Can't execute a JS component");
}

} // flappy
