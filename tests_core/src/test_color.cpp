#include "catch.hpp"
#include "fakeit.hpp"

#include <memory>

#include <core/color.h>

using namespace flappy;
using namespace fakeit;
using namespace std;

template <typename T>
bool Compare(T a, T b)
{
    return fabs(a - b) < std::numeric_limits<T>::epsilon();
}

TEST_CASE( "Color::Color()" ) {
    auto color = Color{0.1f, 0.2f, 0.3f, 1.0f};
    REQUIRE(Compare(color.r(), 0.1f));
    REQUIRE(Compare(color.g(), 0.2f));
    REQUIRE(Compare(color.b(), 0.3f));
    REQUIRE(Compare(color.a(), 1.0f));
}

TEST_CASE( "Color::fromHex()" ) {
    auto color = Color::fromHex(0x00FF00FFu);
    REQUIRE(Compare(color.r(), 0.0f));
    REQUIRE(Compare(color.g(), 1.0f));
    REQUIRE(Compare(color.b(), 0.0f));
    REQUIRE(Compare(color.a(), 1.0f));
}

TEST_CASE( "Color::fromRGBA8()" ) {
    auto color = Color::fromRGBA8(0,255,0,255);
    REQUIRE(Compare(color.r(), 0.0f));
    REQUIRE(Compare(color.g(), 1.0f));
    REQUIRE(Compare(color.b(), 0.0f));
    REQUIRE(Compare(color.a(), 1.0f));
}

TEST_CASE( "Color::setRGBA();" ) {
    auto color = Color();
    color.setRGBA(0.1f, 0.2f, 0.3f, 1.0f);
    REQUIRE(Compare(color.r(), 0.1f));
    REQUIRE(Compare(color.g(), 0.2f));
    REQUIRE(Compare(color.b(), 0.3f));
    REQUIRE(Compare(color.a(), 1.0f));
}

TEST_CASE( "Color::setR(), Color::setG(), Color::setB(), Color::setA()" ) {
    auto color = Color{};
    color.setR(0.1f);
    color.setG(0.2f);
    color.setB(0.3f);
    color.setA(1.0f);
    REQUIRE(Compare(color.r(), 0.1f));
    REQUIRE(Compare(color.g(), 0.2f));
    REQUIRE(Compare(color.b(), 0.3f));
    REQUIRE(Compare(color.a(), 1.0f));
}

TEST_CASE( "Color::toString()" ) {
    auto color = Color::fromHex(0x050fFf5e);
    REQUIRE(color.toString() == std::string("050fff5e"));
}

TEST_CASE( "Color::toRGBAHex()" ) {
    auto colorHex = uint32_t(0x050FFF5E);
    auto color = Color::fromHex(colorHex);
    REQUIRE(color.toRGBAHex() == colorHex);
}

