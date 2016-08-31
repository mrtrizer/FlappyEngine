#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/managerlist.h>
#include <core/manager.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

class TestManager: public Manager<TestManager> {
public:
    class IMock {
    public:
        virtual void init() = 0;
        virtual void update(TimeDelta dt) = 0;
    };

    TestManager(IMock* mockComponent = nullptr):
        m_mockComponent (mockComponent)
    {}

    void init() override {
        if (m_mockComponent != nullptr)
            m_mockComponent->init();
    }

    void update(TimeDelta dt) override {
        if (m_mockComponent != nullptr)
            m_mockComponent->update(dt);
    }

private:
    IMock* m_mockComponent;
};

class TestManagerEmpty1: public Manager<TestManagerEmpty1> {};
class TestManagerEmpty2: public Manager<TestManagerEmpty2> {};
class TestManagerOverride: public Manager<TestManager> {};

TEST_CASE( "ManagerList::update()") {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,update));

    auto managerList = make_shared<ManagerList>();
    managerList->update();
    managerList->createMgr<TestManager>(&mock.get());
    managerList->createMgr<TestManagerEmpty1>();
    managerList->update();

    Verify(Method(mock,update)).Exactly(1);
}

TEST_CASE( "ManagerList::init()") {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,init));

    auto managerList = make_shared<ManagerList>();
    managerList->init();
    managerList->createMgr<TestManager>(&mock.get());
    managerList->createMgr<TestManagerEmpty1>();
    managerList->init();
    managerList->init(); // souldn't be initialized twice

    Verify(Method(mock,init)).Exactly(1);
}


TEST_CASE( "ManagerList::createMgr()") {
    auto managerList = make_shared<ManagerList>();
    managerList->createMgr<TestManager>();
    managerList->createMgr<TestManagerEmpty1>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::overrideMgr()") {
    auto managerList = make_shared<ManagerList>();
    managerList->overrideMgr<TestManager, TestManagerOverride>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManagerOverride));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "ManagerList::MGR()") {
    auto managerList = make_shared<ManagerList>();
    REQUIRE(managerList->MGR<TestManager>() == nullptr);
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
    managerList->createMgr<TestManager>();
    managerList->init();
    REQUIRE(typeid(*managerList->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(managerList->MGR<TestManagerEmpty2>() == nullptr);
}

//TEST_CASE( "ManagerList::args()") {
//    auto managerList1 = make_shared<ManagerList>();
//    REQUIRE(string(managerList1->args()[0]) == "flappy");
//    const char* args[] = {"test", "test_arg"};
//    auto managerList2 = make_shared<ManagerList>(2, args);
//    REQUIRE(string(managerList2->args()[1]) == "test_arg");
//}
