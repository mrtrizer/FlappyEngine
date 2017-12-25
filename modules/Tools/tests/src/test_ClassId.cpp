#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <TypeId.h>

using namespace flappy;
using namespace std;

struct A {};
struct B: public A {};
struct C: public B {};

TEST_CASE( "ClassId::id()") {
#ifndef SHARED_TYPE_ID
    REQUIRE((GetTypeId<A,A>::value() == TypeId<A>(0)));
    REQUIRE((GetTypeId<A,B>::value() == TypeId<A>(1)));
    REQUIRE((GetTypeId<A,C>::value() == TypeId<A>(2)));
    REQUIRE((GetTypeId<A,B>::value() == TypeId<A>(1)));
    REQUIRE(TypeCounter<A>::count() == 3);
#endif
}
