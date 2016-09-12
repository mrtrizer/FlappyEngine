#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/scene.h>

using namespace flappy;
using namespace std;

struct A {};
struct B: public A {};
struct C: public B {};

TEST_CASE( "ClassId::id()") {
    REQUIRE((ClassId<A,A>::id() == 0));
    REQUIRE((ClassId<A,B>::id() == 1));
    REQUIRE((ClassId<A,C>::id() == 2));
    REQUIRE((ClassId<A,B>::id() == 1));
}
