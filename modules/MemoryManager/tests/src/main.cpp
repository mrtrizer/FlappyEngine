#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ObjectPool.hpp>
#include <Handle.hpp>
#include <ObjectPoolDebugger.hpp>

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

TEST_CASE( "Adding of Component to Entity") {

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

            auto strongHandle2 = objectPool.create<Test2>(20, "Fuck");
            REQUIRE(debugger.getChankIndex(strongHandle2) == 2);
            b = strongHandle;

            auto otherStrongHandle = std::move(strongHandle);
            REQUIRE(debugger.getChankIndex(otherStrongHandle) == 1);
            REQUIRE(b->value() == 20);

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
