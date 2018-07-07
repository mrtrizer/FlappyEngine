#include <iostream>

#include <catch.h>
#include <fakeit.h>

#include <ChankArray.hpp>
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

TEST_CASE( "Adding of Component to Entity") {

    ChankArray<Test> chankArray(10);
    {
        auto a = chankArray.create(10);
        REQUIRE(a->value() == 10);
        Handle<Test> b = nullptr;
        {
            auto strongHandle = chankArray.create(20);
            b = strongHandle;
            auto otherStrongHandle = std::move(strongHandle);
            REQUIRE(b->value() == 20);
        }
        REQUIRE_THROWS(b->value());
        auto c = chankArray.create(30);
        REQUIRE(c->value() == 30);
    }
}
