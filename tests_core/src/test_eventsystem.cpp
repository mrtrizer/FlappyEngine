#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <core/eventsystem.h>
#include <core/tools.h>

using namespace flappy;
using namespace fakeit;

struct IMockHandler {
    virtual void func() = 0;
};

TEST_CASE( "EventSystem::subscribe() EventSystem::post() EventSystem::exec() [queued call]") {
    Mock<IMockHandler> mock;
    Fake(Method(mock,func));

    struct TestEvent {};
    EventSystem eventSystem;
    eventSystem.subscribe([&mock](TestEvent) {
        mock.get().func();
    });
    eventSystem.post(TestEvent());
    eventSystem.exec();

    Verify(Method(mock,func)).Exactly(1);
}

