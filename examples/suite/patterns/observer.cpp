// Vector observer example.
//
// Demonstrates:
// - subscribing to mutation events
// - reading event payloads
// - multiple listeners
// - unsubscribing listeners

#include <support/framework.h>

using namespace VectorPro;

// Converts event type to a readable string.
static const char* eventName(ObservableVector<int>::EventType type) {
    switch (type) {
    case ObservableVector<int>::EventType::PUSHBACK:
        return "PUSHBACK";
    case ObservableVector<int>::EventType::EMPLACEBACK:
        return "EMPLACEBACK";
    case ObservableVector<int>::EventType::POPBACK:
        return "POPBACK";
    case ObservableVector<int>::EventType::INSERT:
        return "INSERT";
    case ObservableVector<int>::EventType::ERASE:
        return "ERASE";
    case ObservableVector<int>::EventType::REMOVE:
        return "REMOVE";
    case ObservableVector<int>::EventType::CLEAR:
        return "CLEAR";
    case ObservableVector<int>::EventType::RESERVE:
        return "RESERVE";
    case ObservableVector<int>::EventType::SHRINK:
        return "SHRINK";
    }
    return "UNKNOWN";
}

static void run_examples() {
    // Subscribe to mutation events.
    setTitle("Subscribe");

    ObservableVector<int> v;

    (void)v.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData e) {
        std::cout << "Event: " << eventName(e.type) << " | old=" << e.oldSize
                  << " new=" << e.newSize << "\n";
    });

    v.push_back(1);
    v.push_back(2);
    v.emplace_back(3);

    std::cout << "\n";

    // Trigger and observe multiple event types.
    setTitle("Event Flow");

    (void)v.insert(v.cbegin() + 1, 99);
    (void)v.erase(v.cbegin());
    v.reserve(50);
    v.shrink_to_fit();

    std::cout << "\n";

    // Multiple listeners.
    setTitle("Multiple Listeners");

    ObservableVector<int> multi;
    int a = 0, b = 0;

    (void)multi.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++a; });
    (void)multi.subscribe(
        [&](const ObservableVector<int>&, ObservableVector<int>::EventData) { ++b; });

    multi.push_back(1);
    multi.push_back(2);

    std::cout << "Listener A calls : " << a << "\n";
    std::cout << "Listener B calls : " << b << "\n\n";

    // Unsubscribe behavior.
    setTitle("Unsubscribe");

    auto handle =
        multi.subscribe([](const ObservableVector<int>&, ObservableVector<int>::EventData) {
            std::cout << "Third listener fired\n";
        });

    multi.push_back(3);
    multi.unsubscribe(handle);
    multi.push_back(4);

    std::cout << "Third listener removed\n";
}

REGISTER_EXAMPLE_SUITE();