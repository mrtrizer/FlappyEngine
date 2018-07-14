#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ObjectPool.hpp>
#include <Handle.hpp>

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

    ObjectPool<64> objectPool(10);
    {
        auto a = objectPool.create<Test>(10);
        REQUIRE(a->value() == 10);
        Handle<Test> b = nullptr;
        {
            auto strongHandle = objectPool.create<Test>(20);
            auto strongHandle2 = objectPool.create<Test2>(20, "Fuck");
            b = strongHandle;
            auto otherStrongHandle = std::move(strongHandle);
            REQUIRE(b->value() == 20);
        }
        REQUIRE_THROWS(b->value());
        auto c = objectPool.create<Test>(30);
        REQUIRE(c->value() == 30);
    }
}
