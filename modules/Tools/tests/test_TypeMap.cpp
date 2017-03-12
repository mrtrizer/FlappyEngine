#include "catch.h"

#include "TypeMap.h"

using namespace flappy;

TEST_CASE( "TypeMap::TypeMap()") {
    struct BaseClass {
        BaseClass() {}
        BaseClass(int value): value(value) {}
        int value = 0;
    };

    struct Class1: public BaseClass {
        using BaseClass::BaseClass;
    };

    struct Class2: public BaseClass {
        using BaseClass::BaseClass;
    };

    TypeMap<BaseClass, std::shared_ptr<BaseClass>> typeMap;
    typeMap.set<Class1>(std::make_shared<Class1>(10));
    REQUIRE(!typeMap.get<Class2>());
    typeMap.set<Class2>(std::make_shared<Class2>(20));

    REQUIRE(typeMap.get<Class1>()->value == 10);
    REQUIRE(typeMap.get<Class2>()->value == 20);
}
