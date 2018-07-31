#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ObjectPool.hpp>
#include <Handle.hpp>
#include <ObjectPoolDebugger.hpp>
#include <Heap.hpp>

using namespace fakeit;
using namespace std;

class ITest {
public:
    virtual ~ITest() = default;
    virtual int value() = 0;
};

class Test : public ITest {
public:
    Test(int i) : m_i(i) {}
    int value() override { return m_i; }
private:
    int m_i = 0;
};

class Test2 : public EnableSelfHandle<Test2> {
public:
    Test2(int i, std::string text) : m_i(i), m_text(text) {}
//    ~Test2() {

//    }
    int value() { return m_i; }
    Handle<Test2> extractHandle() {
        return selfHandle();
    }
private:
    int m_i = 0;
    std::string m_text;
};

TEST_CASE( "Heap") {
    auto testObject = Heap::create<Test>(10);
    REQUIRE(testObject->value() == 10);
}

TEST_CASE("Self handle") {
    auto testObject = Heap::create<Test2>(100, "Self heap");
    REQUIRE(testObject->extractHandle()->value() == 100);
}

TEST_CASE("StrongHandle derived class cast") {
    auto testObject1 = static_cast<StrongHandle<ITest>>(Heap::create<Test>(10));
    REQUIRE(testObject1->value() == 10);

    testObject1 = Heap::create<Test>(100);
    REQUIRE(testObject1->value() == 100);
}

TEST_CASE("Handle derived class cast") {
    auto strongHandle = Heap::create<Test>(10);

    Handle<ITest> test1 (strongHandle);
    test1 = strongHandle.handle();
}

TEST_CASE( "Constructors, destructors, assignments") {
    ObjectPool objectPool(64, 10);
    ObjectPoolDebugger debugger(objectPool);
    {
        auto a = objectPool.create<Test>(10);
        REQUIRE(debugger.getChankIndex(a) == 0);
        REQUIRE(a->value() == 10);

        debugger.printState();

        Handle<Test> b = nullptr;
        {
            auto strongHandle = objectPool.create<Test>(20);
            REQUIRE(debugger.getChankIndex(strongHandle) == 1);
            strongHandle = nullptr;
            REQUIRE(!strongHandle.isValid());

            auto strongHandle2 = objectPool.create<Test2>(20, "Fuck");
            REQUIRE(debugger.getChankIndex(strongHandle2) == 1);
            auto b2 = strongHandle2.handle();
            b = strongHandle;

            debugger.printState();

            strongHandle = objectPool.create<Test>(30);
            REQUIRE(debugger.getChankIndex(strongHandle) == 2);

            debugger.printState();

            auto otherStrongHandle = std::move(strongHandle);
            REQUIRE(debugger.getChankIndex(otherStrongHandle) == 2);
            REQUIRE(b->value() == 30);

            debugger.printState();
        }
        REQUIRE_THROWS(b->value());

        debugger.printState();

        auto c = objectPool.create<Test>(30);
        REQUIRE(debugger.getChankIndex(c) == 1);
        REQUIRE(c->value() == 30);

        debugger.printState();
    }
}
