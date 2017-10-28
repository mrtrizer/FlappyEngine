#include "JSComponent.h"

#include <Entity.h>
#include <V8JSManager.h>

namespace flappy {

JSComponent::JSComponent(std::string name, std::string script)
{
    addDependency(V8JSManager::id());

    subscribe([this, name, script](InitEvent) {
        m_jsObject = manager<V8JSManager>()->runJSComponent(script);
        //manager<V8JSManager>()->callMethod(m_jsObject, "init");
    });

    subscribe([this, name, script](DeinitEvent) {
        //manager<V8JSManager>()->callMethod(m_jsObject, "deinit");
    });

    subscribe([this, name, script](UpdateEvent e) {
        //manager<V8JSManager>()->callMethod(m_jsObject, "update", {e.dt});
    });
}

} // flappy
