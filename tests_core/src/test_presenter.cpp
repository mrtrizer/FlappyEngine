#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/presenter.h>
#include <core/viewfactory.h>
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
    struct IMock {
        virtual void makeGView() = 0;
    };
    TestPresenter(IMock& mock): m_mock(mock) {}

private:
    std::shared_ptr<View> makeGView(const ViewFactory&) override {
        m_mock.makeGView();
        return std::make_shared<TestView>();
    }

    IMock& m_mock;
};

TEST_CASE("Presenter::getGView") {
    Mock<TestPresenter::IMock> mock;
    Fake(Method(mock,makeGView));
    auto testPresenter = std::make_shared<TestPresenter> (mock.get());
    testPresenter->getGView(ViewFactory());
    testPresenter->getGView(ViewFactory());
    Verify(Method(mock,makeGView)).Exactly(1);
}

TEST_CASE("Presenter::cleanGView") {
    Mock<TestPresenter::IMock> mock;
    Fake(Method(mock,makeGView));
    auto testPresenter = std::make_shared<TestPresenter> (mock.get());
    testPresenter->getGView(ViewFactory());
    testPresenter->cleanGView();
    testPresenter->getGView(ViewFactory());
    Verify(Method(mock,makeGView)).Exactly(2);
}

TEST_CASE("Presenter::setColor() Presenter::color()") {
    Mock<TestPresenter::IMock> mock;
    auto testPresenter = std::make_shared<TestPresenter> (mock.get());
    auto color = Color{0.1f,0.2f,0.3f,0.4f};
    testPresenter->setColor(color);
    REQUIRE(testPresenter->color().r() == color.r());
    REQUIRE(testPresenter->color().g() == color.g());
    REQUIRE(testPresenter->color().b() == color.b());
    REQUIRE(testPresenter->color().a() == color.a());
}
