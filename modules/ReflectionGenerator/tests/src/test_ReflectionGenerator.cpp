#include <memory>
#include <iostream>

#include <catch.h>

#include "ReflectedClass.hpp"

#include <GeneratedReflection.hpp>

using namespace std;

TEST_CASE("Method list") {
    auto reflectionDb = extractReflectionDb();
    auto type = reflectionDb.getType("ReflectedClass");
    auto instance = type.constructOnStack();
    REQUIRE(type.function("a")(instance, 10).as<int>() == 10);
}

TEST_CASE("Method call") {
}
