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

static std::string lastCalledFuncName;

static void voidReturnNoArgsFunc() {
    lastCalledFuncName = "voidReturnNoArgsFunc";
}

static void voidReturnBasicArgFunc(int) {
    lastCalledFuncName = "voidReturnBasicArgFunc";
}

static int basicReturnFunc() {
    lastCalledFuncName = "basicReturnFunc";
    return 1000;
}

static TestClass classReturnFunc() {
    lastCalledFuncName = "classReturnFunc";
    return TestClass(10);
}

static TestClass* pointerReturnFunc() {
    lastCalledFuncName = "classReturnFunc";
    static TestClass test(10);
    return &test;
}

static TestClass& refReturnFunc() {
    lastCalledFuncName = "refReturnFunc";
    static TestClass test(10);
    return test;
}

static int multiArgFunc(int a, TestClass test, TestClass* testPtr, TestClass& testRef) {
    testRef.m_c = testPtr->m_c = a + test.m_c;
    return testRef.m_c;
}


static void testFunc(std::string str) {
    std::cout << str << std::endl;
}

TEST_CASE("Function") {
    auto reflection = std::make_shared<Reflection>(BasicTypesReflection::instance().reflection());

    reflection->registerFunction("voidReturnNoArgsFunc", &voidReturnNoArgsFunc);
    REQUIRE_NOTHROW(reflection->getFunction("voidReturnNoArgsFunc")());
    REQUIRE(lastCalledFuncName == "voidReturnNoArgsFunc");
}
