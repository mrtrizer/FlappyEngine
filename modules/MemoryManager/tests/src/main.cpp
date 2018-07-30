#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ObjectPool.hpp>
#include <Handle.hpp>
#include <ObjectPoolDebugger.hpp>
#include <Heap.hpp>

using namespace fakeit;
using namespace std;

class Test {
public:
    Test(int i) : m_i(i) {}
    int value() { return m_i; }
private:
    int m_i = 0;
};

class Test2 {
public:
    Test2(int i, std::string text) : m_i(i), m_text(text) {}
    int value() { return m_i; }
private:
    int m_i = 0;
    std::string m_text;
};

TEST_CASE( "Heap") {
    auto testObject = Heap::create<Test>(10);
    REQUIRE(testObject->value() == 10);
}

// TODO: Test self handle

// TODO: Test derived type casting

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
