#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/managerlist.h>
#include <core/presenter.h>

#include "testview.h"

using namespace flappy;
using namespace std;
using namespace fakeit;
using namespace glm;


TEST_CASE( "View::redraw()") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));

    TestView testView(&mock.get());
    testView.redraw({}, {});

    Verify(Method(mock, draw)).Exactly(1);
}

TEST_CASE( "View::externUpdate()") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));
    Fake(Method(mock, update));

    TestView testView(&mock.get());
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

    TestView testView(&mock.get());
    { // CircleShape will be deleted at the end of block
        auto circleShape = std::make_shared<CircleShape>();
        testView.externUpdate(circleShape);
    }
    mat4 pMatrix;
    mat4 mvMatrix;
    testView.redraw(pMatrix, mvMatrix);

    Verify(Method(mock, draw)).Exactly(1);
}
