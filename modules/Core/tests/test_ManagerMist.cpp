#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <Manager.h>

#include "TestManager.h"

using namespace flappy;
using namespace fakeit;
using namespace std;

TEST_CASE( "ManagerList::init() ManagerList::update()") {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,update));
    Fake(Method(mock,init));

    auto managerList = make_shared<ManagerList>();
    managerList->update();
    managerList->create<TestManager>(&mock.get());
    managerList->create<TestManagerEmpty1>();
    managerList->init();
    managerList->update();

    Verify(Method(mock,init), Method(mock,update)).Exactly(1);
}

TEST_CASE( "ManagerList::create() ManagerList::remove()") {
    auto managerList = make_shared<ManagerList>();
    managerList->create<TestManager>();
    managerList->create<TestManagerEmpty1>();
    managerList->init();
    REQUIRE(typeid(*managerList->manager<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->manager<TestManagerEmpty2>() == nullptr);
    managerList->remove<TestManager>();
    REQUIRE(managerList->manager<TestManager>() == nullptr);
}

TEST_CASE( "ManagerList::override()") {
    auto managerList = make_shared<ManagerList>();
    managerList->override<TestManager, TestManagerOverride>();
    managerList->init();
    REQUIRE(typeid(*managerList->manager<TestManager>()) == typeid(TestManagerOverride));
    REQUIRE(managerList->manager<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::manager() [one ManagerList]") {
    auto managerList = make_shared<ManagerList>();
    REQUIRE(managerList->manager<TestManager>() == nullptr);
    REQUIRE(managerList->manager<TestManagerEmpty2>() == nullptr);
    managerList->create<TestManager>();
    managerList->init();
    REQUIRE(typeid(*managerList->manager<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->manager<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::manager() [nested ManagerLists]") {
    auto managerList1 = make_shared<ManagerList>();
    managerList1->create<TestManager>();
    managerList1->init();
    auto managerList2 = make_shared<ManagerList>();
    managerList2->setParent(managerList1);
    managerList2->create<TestManagerEmpty2>();
    managerList2->init();
    REQUIRE(typeid(*managerList2->manager<TestManager>()) == typeid(TestManager));
    REQUIRE(typeid(*managerList2->manager<TestManagerEmpty2>())  == typeid(TestManagerEmpty2));
}
