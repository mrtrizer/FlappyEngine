#pragma once

#include <Component.h>
#include <View.h>

#include "RenderElementFactory.h"

namespace flappy {

class MeshComponent: public Component {
protected:
    MeshComponent(): Component({RenderElementFactory::id()})
    {}

    void init() override {
        m_renderElement = manager<RenderElementFactory>()->createRect();
        entity()->addComponent(m_renderElement);
    }

    void deinit() override {
        entity()->removeComponent(m_renderElement);
        m_renderElement.reset();
    }

private:
    std::shared_ptr<View> m_renderElement;
};

} // flappy
