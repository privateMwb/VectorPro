// Vector Listener Handle Regression Suite
// Verifies the consequences of unsubscribe()'s compaction strategy: removing
// listener `handle` shifts every listener after it one slot to the left
// (listeners_[i] = move(listeners_[i+1]) for i from handle upward). Any
// handle previously issued for one of those shifted listeners becomes
// stale — it now identifies a different listener than the one the caller
// originally received it for, or it falls out of the valid range entirely
// and silently does nothing.
//
// Covers:
// - unsubscribing an earlier handle silently reassigns a later handle's
//   meaning to a different listener
// - the handle that fell out of range can no longer remove the listener it
//   now identifies, which keeps firing indefinitely
// - a handle that stays in range after the shift ends up removing a
//   different listener than the one it was originally issued for

#include <support/framework.h>

using namespace VectorPro;

// Verifies that removing the first listener shifts the third listener's
// position down to index 1, and that the handle originally issued for the
// third listener (h2 == 2) is now past the valid range, so it can never be
// used to remove that listener again.
static void unsubscribe_earlier_handle_strands_later_handle() {
    ObservableVector<int> v;

    int count0 = 0, count1 = 0, count2 = 0;
    auto h0 = v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count0; });
    (void)h0;
    (void)v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count1; });
    auto h2 = v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count2; });

    v.unsubscribe(0);
    // Shift: index 0 <- old index 1 (h1's callback), index 1 <- old index 2
    // (h2's callback). lsize_ is now 2, so h2's original handle value (2)
    // is already out of range.

    v.push_back(1);
    CHK(count0 == 0); // correctly removed
    CHK(count1 == 1); // shifted to index 0, still fires
    CHK(count2 == 1); // shifted to index 1, still fires

    // h2 (== 2) is now >= lsize_ (2), so this is a silent no-op: the
    // listener that used to be identified by h2 can never be removed
    // through the handle the caller was originally given for it.
    v.unsubscribe(h2);
    v.push_back(2);

    CHK(count2 == 2); // still fires despite the "unsubscribe" call above
}

// Verifies that a handle which *does* remain in range after an earlier
// removal ends up identifying a different listener than the one it was
// originally issued for.
static void unsubscribe_shifted_handle_removes_wrong_listener() {
    ObservableVector<int> v;

    int count0 = 0, count1 = 0, count2 = 0;
    (void)v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count0; });
    auto h1 = v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count1; });
    (void)v.subscribe(
        [&](const ObservableVector<int>&, const ObservableVector<int>::EventData&) { ++count2; });

    v.unsubscribe(0);
    // Shift: index 0 <- h1's callback, index 1 <- h2's callback. lsize_ == 2.
    // h1's original handle value is 1, which is still in range -- but index
    // 1 now holds h2's callback, not h1's.

    v.unsubscribe(h1);
    // This removes whatever now lives at index 1: h2's callback, not h1's.

    v.push_back(1);

    CHK(count0 == 0); // correctly removed at the start
    CHK(count1 == 1); // h1's own callback is untouched, now at index 0
    CHK(count2 == 0); // removed under a handle that was never issued for it
}

// Executes all listener handle regression test cases.
static void run_tests() {
    RUN(unsubscribe_earlier_handle_strands_later_handle);
    RUN(unsubscribe_shifted_handle_removes_wrong_listener);
}

REGISTER_TEST_SUITE();
