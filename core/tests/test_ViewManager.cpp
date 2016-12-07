#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <ManagerList.h>
#include <ViewManager.h>
#include <PresenterComponent.h>
#include <ScreenManager.h>
#include <SceneManager.h>

#include "testview.h"
#include "testscene.h"

using namespace flappy;
using namespace std;
using namespace fakeit;
using namespace glm;


class TestViewManager: public ViewManager {
protected:
    virtual void redraw(std::list<Visual> & list, glm::mat4 & pMatrix) {
        for (auto visual: list)
            visual.view->redraw(pMatrix, visual.pos);
    }

private:
    virtual void updateViewPort() {
    }
};

TEST_CASE( "ViewManager::update()") {
    Mock<TestView::IMock> mock;

    Fake(Method(mock, draw));
    Fake(Method(mock, update));

    auto managerList = std::make_shared<ManagerList>();
    managerList->create<ScreenManager>(); // ViewManager depends from ScreenManager
    auto sceneManager = managerList->create<SceneManager>(); // ViewManager dependes from SceneManager
    sceneManager->setScene(std::make_shared<TestScene>());
    auto entityManager = managerList->create<EntityManager>();
    auto entity = entityManager->create(); // ViewManager can't render without camera
    auto camera = entity->create<CameraComponent>();
    auto viewManager = managerList->override<ViewManager,TestViewManager>();

    REQUIRE(viewManager != nullptr);

    viewManager->bind<CircleShape, TestView>();

    auto circleEntity = entityManager->create();
    auto circleShape = circleEntity->create<CircleShape>();

    auto view = static_pointer_cast<TestView>(circleShape->view().lock());

    view->setMock(&mock.get());

    managerList->update();
    managerList->update();

    Verify(Method(mock,update), Method(mock, draw)).Exactly(1);

}

TEST_CASE( "ViewManager::resize()") {
}

TEST_CASE( "ViewManager::updateSize()") {
}
