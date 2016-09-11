#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/manager.h>

#include "testmanager.h"

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

TEST_CASE( "ManagerList::create()") {
    auto managerList = make_shared<ManagerList>();
    managerList->create<TestManager>();
    managerList->create<TestManagerEmpty1>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::override()") {
    auto managerList = make_shared<ManagerList>();
    managerList->override<TestManager, TestManagerOverride>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManagerOverride));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::MGR() [one ManagerList]") {
    auto managerList = make_shared<ManagerList>();
    REQUIRE(managerList->MGR<TestManager>() == nullptr);
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
    managerList->create<TestManager>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::MGR() [nested ManagerLists]") {
    auto managerList1 = make_shared<ManagerList>();
    managerList1->create<TestManager>();
    managerList1->init();
    auto managerList2 = make_shared<ManagerList>();
    managerList2->setParent(managerList1);
    managerList2->create<TestManagerEmpty2>();
    managerList2->init();
    REQUIRE(typeid(*managerList2->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(typeid(*managerList2->MGR<TestManagerEmpty2>())  == typeid(TestManagerEmpty2));
}
