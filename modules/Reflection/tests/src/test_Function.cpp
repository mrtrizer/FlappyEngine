#include <iostream>
#include <catch.h>

#include "Function.hpp"
#include "Type.hpp"
#include "BasicTypesReflection.hpp"

using namespace flappy;

// Test functions

struct TestClass {
    TestClass(int c)
        : m_c(c)
    {}
    int testMethodConst(int a, int b) const {
        return a * b * m_c;
    }

    int testMethod(int a, int b) {
        return a * b * m_c;
    }
    
    int m_c = 0;
};

static void* lastCalledFunc;
static int lastPassedIntValue = 0;

static void voidReturnNoArgsFunc() {
    lastCalledFunc = (void*)&voidReturnNoArgsFunc;
}

TEST_CASE("Function_voidReturnNoArgsFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("voidReturnNoArgsFunc", &voidReturnNoArgsFunc);
    reflection->getFunction("voidReturnNoArgsFunc")();
    REQUIRE(lastCalledFunc == (void*)&voidReturnNoArgsFunc);
}

static void voidReturnBasicArgFunc(int value) {
    lastCalledFunc = (void*)&voidReturnBasicArgFunc;
    lastPassedIntValue = value;
}

TEST_CASE("Function_voidReturnBasicArgFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("voidReturnBasicArgFunc", &voidReturnBasicArgFunc);
    reflection->getFunction("voidReturnBasicArgFunc")(10);
    REQUIRE(lastPassedIntValue == 10);
}

static int basicReturnFunc() {
    lastCalledFunc = (void*)&basicReturnFunc;
    return 1000;
}

TEST_CASE("Function_basicReturnFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("basicReturnFunc", &basicReturnFunc);
    REQUIRE(reflection->getFunction("basicReturnFunc")().as<int>() == 1000);
}

static TestClass classReturnFunc() {
    lastCalledFunc = (void*)&classReturnFunc;
    return TestClass(20);
}

TEST_CASE("Function_classReturnFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("classReturnFunc", &classReturnFunc);
    REQUIRE(reflection->getFunction("classReturnFunc")().as<TestClass&>().m_c == 20);
}

static TestClass* pointerReturnFunc() {
    lastCalledFunc = (void*)&classReturnFunc;
    static TestClass test(100);
    return &test;
}

TEST_CASE("Function_pointerReturnFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("pointerReturnFunc", &pointerReturnFunc);
    REQUIRE(reflection->getFunction("pointerReturnFunc")().as<TestClass*>()->m_c == 100);
}

static TestClass& refReturnFunc() {
    lastCalledFunc = (void*)&refReturnFunc;
    static TestClass test(1000);
    return test;
}

TEST_CASE("Function_refReturnFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("refReturnFunc", &refReturnFunc);
    REQUIRE(reflection->getFunction("refReturnFunc")().as<TestClass&>().m_c == 1000);
}

static int multiArgFunc(int a, TestClass test, TestClass* testPtr, TestClass& testRef) {
    testRef.m_c = testPtr->m_c = a + test.m_c;
    return testRef.m_c;
}

TEST_CASE("Function_multiArgFunc") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());
    reflection->registerFunction("multiArgFunc", &multiArgFunc);
    TestClass test1(1);
    TestClass test2(2);
    TestClass test3(3);
    REQUIRE_NOTHROW(reflection->getFunction("multiArgFunc")(10, test1, &test2, test3));
    REQUIRE(test2.m_c == 11);
    REQUIRE(test3.m_c == 11);
}
