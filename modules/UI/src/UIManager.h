#pragma once

#include <Manager.h>

namespace flappy {

class TouchComponent;

class UIManager: public Manager<UIManager>
{
public:
    UIManager();
    void registerTouchComponent(SafePtr<TouchComponent> touchComponent);
    void unregisterTouchComponent(SafePtr<TouchComponent> touchComponent);
};

} // flappy
