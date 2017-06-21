#include "catch.h"

#include "SafePtr.h"

using namespace flappy;

TEST_CASE( "SafePtr::SafePtr(std::shared_ptr<T>), SafePtr::operator= (std::shared_ptr<T>), SafePtr::operator->()") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    auto testStruct = std::make_shared<TestStruct>(10);
    auto safePtr = SafePtr<TestStruct>(testStruct);
    REQUIRE(safePtr->value == 10);

    auto safePtr2 = SafePtr<TestStruct>();
    REQUIRE_THROWS(safePtr2->value = 20);
    safePtr2 = testStruct;
    REQUIRE(safePtr2->value == 10);

    testStruct = nullptr;
    REQUIRE_THROWS(safePtr2->value = 20);
    REQUIRE_THROWS(safePtr->value = 20);
}

TEST_CASE( "SafePtr::SafePtr(std::weak_ptr<T>)") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    auto testStruct = std::make_shared<TestStruct>(10);
    auto weakPtr = std::weak_ptr<TestStruct>(testStruct);
    auto safePtr = SafePtr<TestStruct>(weakPtr);
    REQUIRE(safePtr->value == 10);
}

TEST_CASE( "SafePtr::SafePtr(SafePtr<SafePatrT>)") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    struct TestStructChild: public TestStruct {
        using TestStruct::TestStruct;
    };

    auto testStruct = std::make_shared<TestStructChild>(10);
    auto safePtr1 = SafePtr<TestStructChild>(testStruct);
    auto safePtr2 = SafePtr<TestStruct>(safePtr1);
    REQUIRE(safePtr2->value == 10);
}

TEST_CASE( "SafePtr::operator== (std::shared_ptr<T>)") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    auto testStruct = std::make_shared<TestStruct>(10);
    auto safePtr = SafePtr<TestStruct>(testStruct);
    REQUIRE(safePtr == testStruct);
}

TEST_CASE( "SafePtr::operator== (T*)") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    auto testStruct = std::make_shared<TestStruct>(10);
    auto safePtr = SafePtr<TestStruct>(testStruct);
    REQUIRE(safePtr == testStruct.get());
}

TEST_CASE( "SafePtr::operator bool()") {
    struct TestStruct {
        TestStruct(int value): value(value) {}
        int value;
    };

    auto testStruct = std::make_shared<TestStruct>(10);
    auto safePtr = SafePtr<TestStruct>(testStruct);
    REQUIRE(safePtr == true);
}
