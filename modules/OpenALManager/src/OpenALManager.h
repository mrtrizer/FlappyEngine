#pragma once

#include <Manager.h>

namespace flappy {

class OpenALListenerComponent;

class OpenALManager: public Manager<OpenALManager> {
public:
    OpenALManager();

    void registerListener(SafePtr<OpenALListenerComponent> listener);
    void unregisterListener(SafePtr<OpenALListenerComponent> listener);

protected:
    bool isReady() const final;

    SafePtr<OpenALListenerComponent> m_listener;
};

} // flappy
