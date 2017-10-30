#include "JSComponent.h"

#include <Entity.h>
#include <V8JSManager.h>
#include <v8.h>

namespace flappy {

using namespace v8;

JSComponent::JSComponent(std::string name, std::string script)
    : m_name(name)
    , m_script(script)
{
    addDependency(V8JSManager::id());

    subscribe([this, name, script](InitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        m_jsObject = manager<V8JSManager>()->runJSComponent(name, script, selfPointer());
        auto jsObject = v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject);
        manager<V8JSManager>()->callMethod(jsObject, "init", {});
    });

    subscribe([this, name, script](DeinitEvent) {
        HandleScope handleScope(Isolate::GetCurrent());

        manager<V8JSManager>()->callMethod(v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject), "deinit", {});
    });

    subscribe([this, name, script](UpdateEvent e) {
        HandleScope handleScope(Isolate::GetCurrent());

        auto jsDt = v8::Number::New(Isolate::GetCurrent(), e.dt);

        manager<V8JSManager>()->callMethod(v8::Local<v8::Object>::New(v8::Isolate::GetCurrent(), m_jsObject), "update", {jsDt});
    });
}

} // flappy
