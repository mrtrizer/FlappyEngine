#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/flappyapp.h>
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

TEST_CASE( "FlappyApp::update()") {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,update));

    auto flappyApp = make_shared<FlappyApp>();
    flappyApp->update();
    flappyApp->createMgr<TestManager>(&mock.get());
    flappyApp->createMgr<TestManagerEmpty1>();
    flappyApp->update();

    Verify(Method(mock,update)).Exactly(1);
}

TEST_CASE( "FlappyApp::init()") {
    Mock<TestManager::IMock> mock;
    Fake(Method(mock,init));

    auto flappyApp = make_shared<FlappyApp>();
    flappyApp->init();
    flappyApp->createMgr<TestManager>(&mock.get());
    flappyApp->createMgr<TestManagerEmpty1>();
    flappyApp->init();
    flappyApp->init(); // souldn't be initialized twice

    Verify(Method(mock,init)).Exactly(1);
}


TEST_CASE( "FlappyApp::createMgr()") {
    auto flappyApp = make_shared<FlappyApp>();
    flappyApp->createMgr<TestManager>();
    flappyApp->createMgr<TestManagerEmpty1>();
    flappyApp->init();
    REQUIRE(typeid(*flappyApp->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(flappyApp->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "FlappyApp::overrideMgr()") {
    auto flappyApp = make_shared<FlappyApp>();
    flappyApp->overrideMgr<TestManager, TestManagerOverride>();
    flappyApp->init();
    REQUIRE(typeid(*flappyApp->MGR<TestManager>()) == typeid(TestManagerOverride));
    REQUIRE(flappyApp->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "FlappyApp::MGR()") {
    auto flappyApp = make_shared<FlappyApp>();
    REQUIRE(flappyApp->MGR<TestManager>() == nullptr);
    REQUIRE(flappyApp->MGR<TestManagerEmpty2>() == nullptr);
    flappyApp->createMgr<TestManager>();
    flappyApp->init();
    REQUIRE(typeid(*flappyApp->MGR<TestManager>()) == typeid(TestManager));
    REQUIRE(flappyApp->MGR<TestManagerEmpty2>() == nullptr);
}

TEST_CASE( "FlappyApp::args()") {
    auto flappyApp1 = make_shared<FlappyApp>();
    REQUIRE(string(flappyApp1->args()[0]) == "flappy");
    const char* args[] = {"test", "test_arg"};
    auto flappyApp2 = make_shared<FlappyApp>(2, args);
    REQUIRE(string(flappyApp2->args()[1]) == "test_arg");
}
