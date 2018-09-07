#pragma once

namespace flappy {

class IManager {
public:
    virtual ~IManager() = default;
    virtual void update(float dt) {}
};

} // flappy
