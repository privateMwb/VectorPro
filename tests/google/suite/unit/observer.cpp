// Vector Observer Test Suite
// Verifies event subscription, unsubscription, and event payload delivery.
//
// Covers:
// - subscription notifications for all modifying operations
// - event payload contents
// - multiple subscribers
// - unsubscribe
// - unsubscribing an invalid handle
// - unsubscribing one of multiple subscribers
// - operations with no subscribers

#include <gtest/gtest.h>

#include <VectorPro/Vector.h>

using namespace VectorPro;

// Verifies subscribing receives a PUSHBACK event.
TEST(Observer, SubscribeFiresOnPushBack) {
    ObservableVector<int> v;
    int callCount = 0;

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        ++callCount;
        EXPECT_EQ(e.type, ObservableVector<int>::EventType::PUSHBACK);
    });

    v.push_back(1);
    EXPECT_EQ(callCount, 1);
}

// Verifies event payload fields reflect the previous and current vector size.
TEST(Observer, SubscribePayloadValues) {
    ObservableVector<int> v;
    ObservableVector<int>::EventData captured{};

    (void)v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData e) { captured = e; });

    v.push_back(1);
    v.push_back(2);

    EXPECT_EQ(captured.oldSize, 1u);
    EXPECT_EQ(captured.newSize, 2u);
}

// Verifies subscribing receives an EMPLACEBACK event.
TEST(Observer, SubscribeFiresOnEmplaceBack) {
    ObservableVector<int> v;
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    v.emplace_back(5);
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::EMPLACEBACK);
}

// Verifies subscribing receives a POPBACK event.
TEST(Observer, SubscribeFiresOnPopBack) {
    ObservableVector<int> v{1, 2, 3};
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    v.pop_back();
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::POPBACK);
}

// Verifies subscribing receives an INSERT event.
TEST(Observer, SubscribeFiresOnInsert) {
    ObservableVector<int> v{1, 2, 3};
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    (void)v.insert(v.cbegin() + 1, 99);
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::INSERT);
}

// Verifies subscribing receives an ERASE event.
TEST(Observer, SubscribeFiresOnErase) {
    ObservableVector<int> v{1, 2, 3};
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    (void)v.erase(v.cbegin());
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::ERASE);
}

// Verifies subscribing receives a REMOVE event.
TEST(Observer, SubscribeFiresOnRemoveIf) {
    ObservableVector<int> v{1, 2, 3, 4};
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    (void)v.remove_if([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::REMOVE);
}

// Verifies subscribing receives a CLEAR event.
TEST(Observer, SubscribeFiresOnClear) {
    ObservableVector<int> v{1, 2, 3};
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    v.clear();
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::CLEAR);
}

// Verifies subscribing receives a RESERVE event.
TEST(Observer, SubscribeFiresOnReserve) {
    ObservableVector<int> v;
    ObservableVector<int>::EventType lastType{};

    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    v.reserve(50);
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::RESERVE);
}

// Verifies subscribing receives a SHRINK event.
TEST(Observer, SubscribeFiresOnShrink) {
    ObservableVector<int> v;
    v.reserve(50);
    v.push_back(1);

    ObservableVector<int>::EventType lastType{};
    (void)v.subscribe([&](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        lastType = e.type;
    });

    v.shrink_to_fit();
    EXPECT_EQ(lastType, ObservableVector<int>::EventType::SHRINK);
}

// Verifies all subscribed listeners are notified.
TEST(Observer, MultipleListenersAllFire) {
    ObservableVector<int> v;
    int count1 = 0, count2 = 0;

    (void)v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count1; });
    (void)v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count2; });

    v.push_back(1);

    EXPECT_EQ(count1, 1);
    EXPECT_EQ(count2, 1);
}

// Verifies unsubscribing prevents further notifications.
TEST(Observer, UnsubscribeStopsNotifications) {
    ObservableVector<int> v;
    int count = 0;

    auto handle = v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count; });
    v.push_back(1);
    v.unsubscribe(handle);
    v.push_back(2);

    EXPECT_EQ(count, 1);
}

// Verifies unsubscribing an invalid handle is a no-op.
TEST(Observer, UnsubscribeInvalidHandleIsNoop) {
    ObservableVector<int> v;
    int count = 0;

    (void)v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count; });
    v.unsubscribe(999);
    v.push_back(1);

    EXPECT_EQ(count, 1);
}

// Verifies unsubscribing one listener does not affect the others.
TEST(Observer, UnsubscribeOneOfManyPreservesOthers) {
    ObservableVector<int> v;
    int count1 = 0, count2 = 0;

    auto h1 = v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count1; });
    (void)v.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++count2; });

    v.unsubscribe(h1);
    v.push_back(1);

    EXPECT_EQ(count1, 0);
    EXPECT_EQ(count2, 1);
}

// Verifies modifying operations succeed with no subscribed listeners.
TEST(Observer, NoListenersDoesNotThrow) {
    ObservableVector<int> v;
    v.push_back(1);
    v.pop_back();

    EXPECT_EQ(v.size(), 0u);
}
