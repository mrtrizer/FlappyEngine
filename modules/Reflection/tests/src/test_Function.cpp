#include <iostream>
#include <catch.h>

#include "Function.hpp"
#include "Type.hpp"

using namespace flappy;

// Test functions

static int somePrettyFunction(int a, int b, int& result) {
    std::cout << a << " " << b << std::endl;
    result = a + b;
    return a + b;
}

static void testFunc(std::string str) {
    std::cout << str << std::endl;
}

TEST_CASE("Function") {
    auto reflection = std::make_shared<Reflection>();

    auto wrappedFunc1 = Function(*reflection, &somePrettyFunction);
    int result = 0;
    REQUIRE(wrappedFunc1(10, 20, result).as<int>() == 30);
    REQUIRE(result == 30);

    auto wrappedFunc2 = Function(*reflection, &testFunc);
    wrappedFunc2(std::string("Hello, World!"));
}
