// Vector Observer Test Suite
// Validates the subscription, notification, and unsubscription
// behavior of the observer system.
//
// Covers:
// - subscribe returns a valid handle
// - listener fires on push_back
// - listener fires on pop_back
// - listener fires on insert
// - listener fires on erase
// - listener fires on clear
// - listener fires on reserve
// - listener fires on shrink_to_fit
// - listener fires on remove_if
// - unsubscribe stops notifications
// - multiple listeners fire independently
// - EventData carries correct values

#include "test_helper.h"
#include "Vector.h"

using namespace VectorPro;

// Subscribe Returns Handle
// Verifies subscribe returns a valid listener handle.
static void subscribe_returns_handle() {
    Vector<int> v;
    resetEventCount();
    auto handle = v.subscribe(countListener);
    CHK(handle == 0);
}

// Notify On Push Back
// Verifies listener fires once when push_back is called.
static void notify_on_push_back() {
    Vector<int> v;
    resetEventCount();
    (void)v.subscribe(countListener);
    v.push_back(1);
    CHK(g_eventCount == 1);
}

// Notify On Pop Back
// Verifies listener fires once when pop_back is called.
static void notify_on_pop_back() {
    Vector<int> v = { 1, 2, 3 };
    resetEventCount();
    (void)v.subscribe(countListener);
    v.pop_back();
    CHK(g_eventCount == 1);
}

// Notify On Insert
// Verifies listener fires once when insert is called.
static void notify_on_insert() {
    Vector<int> v = { 1, 2, 3 };
    resetEventCount();
    (void)v.subscribe(countListener);
    (void)v.insert(v.cbegin(), 0);
    CHK(g_eventCount == 1);
}

// Notify On Erase
// Verifies listener fires once when erase is called.
static void notify_on_erase() {
    Vector<int> v = { 1, 2, 3 };
    resetEventCount();
    (void)v.subscribe(countListener);
    (void)v.erase(v.cbegin());
    CHK(g_eventCount == 1);
}

// Notify On Clear
// Verifies listener fires once when clear is called.
static void notify_on_clear() {
    Vector<int> v = { 1, 2, 3 };
    resetEventCount();
    (void)v.subscribe(countListener);
    v.clear();
    CHK(g_eventCount == 1);
}

// Notify On Reserve
// Verifies listener fires once when reserve is called.
static void notify_on_reserve() {
    Vector<int> v;
    resetEventCount();
    (void)v.subscribe(countListener);
    v.reserve(20);
    CHK(g_eventCount == 1);
}

// Notify On Shrink To Fit
// Verifies listener fires once when shrink_to_fit is called.
static void notify_on_shrink_to_fit() {
    Vector<int> v = { 1, 2, 3 };
    resetEventCount();
    v.reserve(20);
    (void)v.subscribe(countListener);
    v.shrink_to_fit();
    CHK(g_eventCount == 1);
}

// Notify On Remove If
// Verifies listener fires once when remove_if is called.
static void notify_on_remove_if() {
    Vector<int> v = { 1, 2, 3, 4, 5 };
    resetEventCount();
    (void)v.subscribe(countListener);
    (void)v.remove_if([](const int& x) { return x % 2 == 0; });
    CHK(g_eventCount == 1);
}

// Unsubscribe Stops Notifications
// Verifies listener no longer fires after unsubscribe.
static void unsubscribe_stops_notifications() {
    Vector<int> v;
    resetEventCount();
    auto handle = v.subscribe(countListener);
    v.push_back(1);
    CHK(g_eventCount == 1);

    v.unsubscribe(handle);
    v.push_back(2);
    CHK(g_eventCount == 1);
}

// Multiple Listeners
// Verifies all subscribed listeners fire independently.
static void multiple_listeners() {
    Vector<int> v;
    resetEventCount();
    (void)v.subscribe(countListener);
    (void)v.subscribe(countListener);
    v.push_back(1);
    CHK(g_eventCount == 2);
}

// Event Data Correct
// Verifies EventData carries correct type, index, oldSize, newSize.
static void event_data_correct() {
    Vector<int> v = { 1, 2, 3 };

    Vector<int>::EventData captured{};
    (void)v.subscribe([&captured](const Vector<int>&, Vector<int>::EventData data) {
        captured = data;
    });

    v.push_back(4);
    CHK(captured.type    == Vector<int>::EventType::PUSHBACK);
    CHK(captured.index   == 3);
    CHK(captured.oldSize == 3);
    CHK(captured.newSize == 4);
}

// Test Runner
// Executes all observer test cases.
void run_observer_tests() {
    setTitle("Observer Tests");

    RUN(subscribe_returns_handle);
    RUN(notify_on_push_back);
    RUN(notify_on_pop_back);
    RUN(notify_on_insert);
    RUN(notify_on_erase);
    RUN(notify_on_clear);
    RUN(notify_on_reserve);
    RUN(notify_on_shrink_to_fit);
    RUN(notify_on_remove_if);
    RUN(unsubscribe_stops_notifications);
    RUN(multiple_listeners);
    RUN(event_data_correct);

    std::cout << "\n";
}