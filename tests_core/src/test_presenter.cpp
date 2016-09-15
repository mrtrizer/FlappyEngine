#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/presenter.h>
#include <core/view.h>

using namespace flappy;
using namespace std;
using namespace fakeit;

class TestView: public View {
public:
    void draw(const glm::mat4&, const glm::mat4&) {}
};

class TestPresenter: public Presenter {
public:
    unsigned id() { return ClassId<Presenter,TestPresenter>::id(); }
};

TEST_CASE("Presenter::setColor() Presenter::color()") {
    auto testPresenter = std::make_shared<TestPresenter> ();
    auto color = Color{0.1f,0.2f,0.3f,0.4f};
    testPresenter->setColor(color);
    REQUIRE(testPresenter->color().r() == color.r());
    REQUIRE(testPresenter->color().g() == color.g());
    REQUIRE(testPresenter->color().b() == color.b());
    REQUIRE(testPresenter->color().a() == color.a());
}
