#include "catch.h"

#include <memory>

#include <EventBus.h>
#include <TypeId.hpp>

using namespace flappy;

TEST_CASE( "EventSystem::subscribe() EventSystem::post()") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    auto subscription1 = eventBus.subscribe([&callCounter](TestEvent) {
        callCounter++; // + 2
    });

    // Subscription life time
    {
        auto subscription2 = eventBus.subscribe([&callCounter](TestEvent) {
            callCounter++; // + 1
        });

        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 3);
}

TEST_CASE( "EventSystem::subscription() EventSystem::FlowStatus") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    // Subscription life time
    {
        auto subscription = eventBus.subscribe(
        [&callCounter](TestEvent) {
            callCounter++; // + 1
        });

        auto subscriptionAll = eventBus.subscribeAll(
            [&callCounter](const EventHandle& handle) {
                if (handle.id() == getTypeId<TestEvent>())
                    callCounter++;
            });

        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 2);
}

TEST_CASE( "EventSystem::subscribeAll() EventSystem::post()") {
    int callCounter = 0;

    struct TestEvent1: public IEvent {};
    struct TestEvent2: public IEvent {};

    EventBus eventBus;

    auto subscriptionAll = eventBus.subscribeAll([&callCounter](const EventHandle& handle) {
        if (handle.id() == getTypeId<TestEvent1>())
            callCounter++;
    });

    eventBus.post(TestEvent1());
    eventBus.post(TestEvent2());

    REQUIRE(callCounter == 1);
}

