#pragma once

#include <Component.h>

#include <Box2D/Box2D.h>

namespace flappy {

class Box2DBodyComponent: public Component<Box2DBodyComponent> {
public:
    Box2DBodyComponent();

    b2Body& body() {
        if (m_body == nullptr)
            throw std::runtime_error("Box2DWorldManager is not found in hierarchy.");
        return *m_body;
    }

private:
    void init() override;
    void deinit() override;

    b2Body* m_body;
};

} // flappy
