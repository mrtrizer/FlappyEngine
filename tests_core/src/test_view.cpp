#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/view.h>
#include <core/presenter.h>

using namespace flappy;
using namespace std;
using namespace fakeit;
using namespace glm;

namespace {

class TestView: public View {
public:
    class IMock {
    public:
        void virtual draw() = 0;
        void virtual update() = 0;
    };

    TestView(Mock<IMock>& mock): m_mock(mock) {}

protected:
    void draw(const mat4&, const mat4&) override {
        m_mock.get().draw();
    }
    void update(const Presenter&) override {
        m_mock.get().update();
    }
private:
    Mock<IMock>& m_mock;
};

}

TEST_CASE( "View::redraw()") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));
    Fake(Method(mock, update));

    TestView testView(mock);
    mat4 pMatrix;
    mat4 mvMatrix;
    testView.redraw(pMatrix, mvMatrix);

    Verify(Method(mock, draw)).Exactly(1);
}

TEST_CASE( "View::externUpdate()") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));
    Fake(Method(mock, update));

    TestView testView(mock);
    auto circleShape = std::make_shared<CircleShape>();
    testView.externUpdate(circleShape);
    mat4 pMatrix;
    mat4 mvMatrix;
    testView.redraw(pMatrix, mvMatrix);

    Verify(Method(mock, update), Method(mock, draw)).Exactly(1);
}

TEST_CASE( "View::externUpdate() deleted Presenter") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));

    TestView testView(mock);
    { // CircleShape will be deleted at the end of block
        auto circleShape = std::make_shared<CircleShape>();
        testView.externUpdate(circleShape);
    }
    mat4 pMatrix;
    mat4 mvMatrix;
    testView.redraw(pMatrix, mvMatrix);

    Verify(Method(mock, draw)).Exactly(1);
}
