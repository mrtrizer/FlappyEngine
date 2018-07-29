#pragma once

class IManager {
public:
    virtual ~IManager() = default;
    virtual void update(float dt) = 0;
};
