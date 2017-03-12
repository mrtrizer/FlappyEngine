#include "catch.h"

#include "SafePtr.h"
#include "Tools.h"

using namespace flappy;

TEST_CASE( "SafePtr::SafePtr()") {
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
