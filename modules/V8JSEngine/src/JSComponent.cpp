#include "JSComponent.h"

#include <v8.h>

#include <Entity.h>
#include <TextRes.h>

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

        call( "deinit");
    });

    subscribe([this, name, script](UpdateEvent e) {
        HandleScope handleScope(Isolate::GetCurrent());

        call("update", e.dt);
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

        call( "deinit");
        m_jsObject.Reset();
    });

    subscribe([this, name, textRes](UpdateEvent e) {
        if (isInitialized()) {
            HandleScope handleScope(Isolate::GetCurrent());

            // reinitialization of a script
            if (m_textRes->lastRes() != m_textRes) {
                m_textRes = m_textRes->lastRes();
                call( "deinit");
                init(name, textRes->text());
            }

            call("update", e.dt);
        }
    });
}

ResultValue JSComponent::field(std::string name) {
    ResultValue resultValue(v8::Isolate::GetCurrent(), *manager<V8JSManager>()->context());
    auto jsObject = v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject);
    resultValue.setValue(manager<V8JSManager>()->getField(jsObject, name));
    return resultValue;
}

void JSComponent::init(std::string name, std::string script) {
    m_jsObject = manager<V8JSManager>()->runJSComponent(name, script, selfPointer());
    if (!m_jsObject.IsEmpty())
        call("init");
    else
        throw std::runtime_error("Can't execute a JS component");
}

} // flappy
