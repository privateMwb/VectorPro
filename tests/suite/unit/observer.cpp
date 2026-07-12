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

#include <common/framework.h>

using namespace VectorPro;

// Verifies subscribing receives a PUSHBACK event.
static void subscribe_fires_on_push_back() {
    Vector<int> v;
    int callCount = 0;

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) {
        ++callCount;
        CHK(e.type == Vector<int>::EventType::PUSHBACK);
    });

    v.push_back(1);
    CHK(callCount == 1);
}

// Verifies event payload fields reflect the previous and current vector size.
static void subscribe_payload_values() {
    Vector<int> v;
    Vector<int>::EventData captured{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { captured = e; });

    v.push_back(1);
    v.push_back(2);

    CHK(captured.oldSize == 1);
    CHK(captured.newSize == 2);
}

// Verifies subscribing receives an EMPLACEBACK event.
static void subscribe_fires_on_emplace_back() {
    Vector<int> v;
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    v.emplace_back(5);
    CHK(lastType == Vector<int>::EventType::EMPLACEBACK);
}

// Verifies subscribing receives a POPBACK event.
static void subscribe_fires_on_pop_back() {
    Vector<int> v{1, 2, 3};
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    v.pop_back();
    CHK(lastType == Vector<int>::EventType::POPBACK);
}

// Verifies subscribing receives an INSERT event.
static void subscribe_fires_on_insert() {
    Vector<int> v{1, 2, 3};
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    (void)v.insert(v.cbegin() + 1, 99);
    CHK(lastType == Vector<int>::EventType::INSERT);
}

// Verifies subscribing receives an ERASE event.
static void subscribe_fires_on_erase() {
    Vector<int> v{1, 2, 3};
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    (void)v.erase(v.cbegin());
    CHK(lastType == Vector<int>::EventType::ERASE);
}

// Verifies subscribing receives a REMOVE event.
static void subscribe_fires_on_remove_if() {
    Vector<int> v{1, 2, 3, 4};
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    (void)v.remove_if([](const int& x) { return x % 2 == 0; });
    CHK(lastType == Vector<int>::EventType::REMOVE);
}

// Verifies subscribing receives a CLEAR event.
static void subscribe_fires_on_clear() {
    Vector<int> v{1, 2, 3};
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    v.clear();
    CHK(lastType == Vector<int>::EventType::CLEAR);
}

// Verifies subscribing receives a RESERVE event.
static void subscribe_fires_on_reserve() {
    Vector<int> v;
    Vector<int>::EventType lastType{};

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    v.reserve(50);
    CHK(lastType == Vector<int>::EventType::RESERVE);
}

// Verifies subscribing receives a SHRINK event.
static void subscribe_fires_on_shrink() {
    Vector<int> v;
    v.reserve(50);
    v.push_back(1);

    Vector<int>::EventType lastType{};
    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData e) { lastType = e.type; });

    v.shrink_to_fit();
    CHK(lastType == Vector<int>::EventType::SHRINK);
}

// Verifies all subscribed listeners are notified.
static void multiple_listeners_all_fire() {
    Vector<int> v;
    int count1 = 0, count2 = 0;

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count1; });
    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count2; });

    v.push_back(1);

    CHK(count1 == 1);
    CHK(count2 == 1);
}

// Verifies unsubscribing prevents further notifications.
static void unsubscribe_stops_notifications() {
    Vector<int> v;
    int count = 0;

    auto handle = v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count; });
    v.push_back(1);
    v.unsubscribe(handle);
    v.push_back(2);

    CHK(count == 1);
}

// Verifies unsubscribing an invalid handle is a no-op.
static void unsubscribe_invalid_handle_is_noop() {
    Vector<int> v;
    int count = 0;

    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count; });
    v.unsubscribe(999);
    v.push_back(1);

    CHK(count == 1);
}

// Verifies unsubscribing one listener does not affect the others.
static void unsubscribe_one_of_many_preserves_others() {
    Vector<int> v;
    int count1 = 0, count2 = 0;

    auto h1 = v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count1; });
    (void)v.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++count2; });

    v.unsubscribe(h1);
    v.push_back(1);

    CHK(count1 == 0);
    CHK(count2 == 1);
}

// Verifies modifying operations succeed with no subscribed listeners.
static void no_listeners_does_not_throw() {
    Vector<int> v;
    v.push_back(1);
    v.pop_back();

    CHK(v.size() == 0);
}

// Executes all observer test cases.
static void run_tests() {
    RUN(subscribe_fires_on_push_back);
    RUN(subscribe_payload_values);
    RUN(subscribe_fires_on_emplace_back);
    RUN(subscribe_fires_on_pop_back);
    RUN(subscribe_fires_on_insert);
    RUN(subscribe_fires_on_erase);
    RUN(subscribe_fires_on_remove_if);
    RUN(subscribe_fires_on_clear);
    RUN(subscribe_fires_on_reserve);
    RUN(subscribe_fires_on_shrink);
    RUN(multiple_listeners_all_fire);
    RUN(unsubscribe_stops_notifications);
    RUN(unsubscribe_invalid_handle_is_noop);
    RUN(unsubscribe_one_of_many_preserves_others);
    RUN(no_listeners_does_not_throw);
}

REGISTER_TEST_SUITE();