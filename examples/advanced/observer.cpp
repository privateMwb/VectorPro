// Vector observer example.
//
// Demonstrates:
// - subscribing to mutation events
// - reading event payloads
// - multiple listeners
// - unsubscribing listeners

#include <common/framework.h>

using namespace VectorPro;

// Converts event type to a readable string.
static const char* eventName(Vector<int>::EventType type) {
    switch (type) {
        case Vector<int>::EventType::PUSHBACK:    return "PUSHBACK";
        case Vector<int>::EventType::EMPLACEBACK: return "EMPLACEBACK";
        case Vector<int>::EventType::POPBACK:     return "POPBACK";
        case Vector<int>::EventType::INSERT:      return "INSERT";
        case Vector<int>::EventType::ERASE:       return "ERASE";
        case Vector<int>::EventType::REMOVE:      return "REMOVE";
        case Vector<int>::EventType::CLEAR:       return "CLEAR";
        case Vector<int>::EventType::RESERVE:     return "RESERVE";
        case Vector<int>::EventType::SHRINK:      return "SHRINK";
    }
    return "UNKNOWN";
}

static void run_examples() {
    // Subscribe to mutation events.
    setTitle("Subscribe");

    Vector<int> v;

    (void)v.subscribe([](const Vector<int>&, Vector<int>::EventData e) {
        std::cout << "Event: " << eventName(e.type)
                  << " | old=" << e.oldSize
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

    Vector<int> multi;
    int a = 0, b = 0;

    (void)multi.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++a; });
    (void)multi.subscribe([&](const Vector<int>&, Vector<int>::EventData) { ++b; });

    multi.push_back(1);
    multi.push_back(2);

    std::cout << "Listener A calls : " << a << "\n";
    std::cout << "Listener B calls : " << b << "\n\n";

    // Unsubscribe behavior.
    setTitle("Unsubscribe");

    auto handle = multi.subscribe([](const Vector<int>&, Vector<int>::EventData) {
        std::cout << "Third listener fired\n";
    });

    multi.push_back(3);
    multi.unsubscribe(handle);
    multi.push_back(4);

    std::cout << "Third listener removed\n";
}

REGISTER_EXAMPLE_SUITE();