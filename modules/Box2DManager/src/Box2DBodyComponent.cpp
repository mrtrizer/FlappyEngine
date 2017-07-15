#include "Box2DBodyComponent.h"

#include "Box2DWorldManager.h"
#include "Box2DFixtureComponent.h"

namespace flappy {

Box2DBodyComponent::Box2DBodyComponent() : Component({Box2DWorldManager::id()}) {

}

void Box2DBodyComponent::init() {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(0.0f, 0.0f);

    m_body = manager<Box2DWorldManager>()->world().CreateBody(&bodyDef);
}

void Box2DBodyComponent::deinit() {
    manager<Box2DWorldManager>()->world().DestroyBody(m_body);
}

} // flappy
