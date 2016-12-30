#include "catch.h"
#include "fakeit.h"

#include <memory>

#include <EventBus.h>
#include <Tools.h>

using namespace flappy;
using namespace fakeit;
namespace {

struct IMockHandler {
    virtual void func() = 0;
};

}

TEST_CASE( "EventSystem::subscribe() EventSystem::post() EventSystem::exec() [queued call]") {
    Mock<IMockHandler> mock;
    Fake(Method(mock,func));

    struct TestEvent {};
    EventBus eventSystem;
    { // Subscription life time
        auto subscription = eventSystem.subscribe([&mock](TestEvent) {
            mock.get().func();
        });
        eventSystem.post(TestEvent());
    }

    Verify(Method(mock,func)).Exactly(1);
}

