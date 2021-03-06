#pragma once

#include <Handle.hpp>

namespace flappy {

class OpenALListenerComponent;

class REFLECT OpenALManager {
public:
    OpenALManager();
    ~OpenALManager();

    void registerListener(Handle<OpenALListenerComponent> listener);
    void unregisterListener(Handle<OpenALListenerComponent> listener);

protected:
    Handle<OpenALListenerComponent> m_listener;
};

} // flappy
