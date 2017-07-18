#include "catch.h"

#include <memory>

#include <EventBus.h>
#include <Tools.h>

using namespace flappy;

TEST_CASE( "EventSystem::subscribeIn() EventSystem::subscribeOut() EventSystem::post()") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    auto subscriptionIn1 = eventBus.subscribeIn([&callCounter](TestEvent) {
        callCounter++; // + 2
    });

    // Subscription life time
    {
        auto subscriptionIn2 = eventBus.subscribeIn([&callCounter](TestEvent) {
            callCounter++; // + 1
        });

        auto subscriptionOut2 = eventBus.subscribeOut([&callCounter](TestEvent) {
            callCounter++; // + 1
        });
        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 4);
}

TEST_CASE( "EventSystem::subscriptionIn() EventSystem::FlowStatus") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    // Subscription life time
    {
        auto subscriptionIn = eventBus.subscribeIn(
                    [&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::BREAK;
        });

        auto subscriptionAll = eventBus.subscribeInAll(
                    [&callCounter](const EventHandle& handle) {
            if (handle.id() == GetTypeId<EventHandle, TestEvent>::value())
                callCounter++;
            return FlowStatus::CONTINUE;
        });

        auto subscriptionOut = eventBus.subscribeOut(
                    [&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::CONTINUE;
        });
        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 2);
}


TEST_CASE( "EventSystem::subscriptionAll() EventSystem::FlowStatus") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    // Subscription life time
    {
        auto subscriptionIn = eventBus.subscribeIn([&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::CONTINUE;
        });

        auto subscriptionAll = eventBus.subscribeInAll([&callCounter](const EventHandle& handle) {
            if (handle.id() == GetTypeId<EventHandle, TestEvent>::value())
                callCounter++;
            return FlowStatus::BREAK;
        });

        auto subscriptionOut = eventBus.subscribeOut([&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::CONTINUE;
        });
        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 2);
}

TEST_CASE( "EventSystem::subscriptionOut() EventSystem::FlowStatus") {
    int callCounter = 0;

    struct TestEvent: public IEvent {};
    EventBus eventBus;

    // Subscription life time
    {
        auto subscriptionIn = eventBus.subscribeIn([&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::CONTINUE;
        });

        auto subscriptionAll = eventBus.subscribeInAll([&callCounter](const EventHandle& handle) {
            if (handle.id() == GetTypeId<EventHandle, TestEvent>::value())
                callCounter++;
            return FlowStatus::CONTINUE;
        });

        auto subscriptionOut = eventBus.subscribeOut([&callCounter](TestEvent) {
            callCounter++; // + 1
            return FlowStatus::BREAK;
        });
        eventBus.post(TestEvent());
    }
    eventBus.post(TestEvent());

    REQUIRE(callCounter == 3);
}

TEST_CASE( "EventSystem::subscribeAll() EventSystem::post()") {
    int callCounter = 0;

    struct TestEvent1: public IEvent {};
    struct TestEvent2: public IEvent {};

    EventBus eventBus;

    auto subscriptionAll = eventBus.subscribeInAll([&callCounter](const EventHandle& handle) {
        if (handle.id() == GetTypeId<EventHandle, TestEvent1>::value())
            callCounter++;
        return FlowStatus::CONTINUE;
    });

    eventBus.post(TestEvent1());
    eventBus.post(TestEvent2());

    REQUIRE(callCounter == 1);
}

