#pragma once

#include <Component.h>

namespace flappy {

class Box2DFixtureComponent: public Component<Box2DFixtureComponent> {
public:
    Box2DFixtureComponent();

private:
    void init() override;
    void deinit() override;
};

} // flappy
