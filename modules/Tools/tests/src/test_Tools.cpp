#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <Tools.h>

using namespace flappy;
using namespace std;

class TestPlainClass {

};

template <typename T>
class TestTemplateClass {

};

template <typename T1, typename T2>
class TestDoubleTemplateClass {

};

TEST_CASE("Tools::isPowOfTwo()") {
    REQUIRE(Tools::isPowOfTwo(8) == true);
    REQUIRE(Tools::isPowOfTwo(13) == false);
}

TEST_CASE("Tools::format()") {
    auto str = Tools::format("Test %d test %s", 5, "123");
    REQUIRE(str == "Test 5 test 123");
}

TEST_CASE("Tools::typeName()") {
    REQUIRE(typeName<TestPlainClass>() == "TestPlainClass");
    REQUIRE(typeName<TestTemplateClass<TestPlainClass>>() == "TestTemplateClass<TestPlainClass>");

    auto name = typeName<TestDoubleTemplateClass<TestPlainClass, TestTemplateClass<TestPlainClass>>>();
    REQUIRE(name == "TestDoubleTemplateClass<TestPlainClass, TestTemplateClass<TestPlainClass> >");
}
