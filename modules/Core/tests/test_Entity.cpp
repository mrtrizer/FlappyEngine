#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Entity.h>
#include <TransformComponent.h>

#include "TestComponent.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

// Base

TEST_CASE( "root() parent()" )
{
}

TEST_CASE( "events() Events sending" )
{
}


// Component managment

TEST_CASE("addComponent(std::shared_ptr<ComponentT> )") {

}

TEST_CASE("createComponent(Args ... args)") {

}

TEST_CASE("removeComponent(std::shared_ptr<ComponentT> )") {

}

TEST_CASE("findComponent(predicate, depth) findComponent(depth)") {

}

TEST_CASE("findComponents(predicate, depth) findComponents(depth)") {

}

TEST_CASE("component<ComponentT>()") {

}


// Manager managment

TEST_CASE("addManager(std::shared_ptr<ManagerT>) manager<ManagerT>()") {

}

TEST_CASE("createManager(Args ... args)") {

}

TEST_CASE("removeManager(std::shared_ptr<ManagerT> manager)") {

}


// Entity managment

TEST_CASE("addEntity(std::shared_ptr<Entity>)") {

}

TEST_CASE("removeEntity(std::shared_ptr<Entity>)") {

}

TEST_CASE("findEntity(predicate, depth)") {

}

TEST_CASE("findEntities(depth) findEntities(predicate, depth)") {

}
